#![no_std]
#![no_main]

use core::{convert::TryInto, panic, str};

extern crate alloc;

#[macro_use]
extern crate user_lib;
// # define LEFT_KEY ''

const LF: u8 = 0x0au8;
const CR: u8 = 0x0du8;
const DL: u8 = 0x7fu8;
const BS: u8 = 0x08u8;
const CSI:u8 = 0x1bu8;
const DEL:u8 = 0x7eu8;

macro_rules! color_text {
    ($text:expr, $color:expr) => {{
        format_args!("\x1b[{}m{}\x1b[0m", $color, $text)
    }};
}

macro_rules! cursor_move_left {
    ($x:literal ) => {
        if $x > 0{
            print!("\x1b[{}D", ($x));//when $x=0, it will still view $x as 1
        }
    };

    ($x:expr ) => {
        if $x > 0{
            print!("\x1b[{}D", ($x));
        }
    };
}
macro_rules! cursor_move_right {
    ($x:literal ) => {
        if $x > 0{
            print!("\x1b[{}C",($x))//when $x=0, it will still view $x as 1
        }
    };
    ($x:expr ) => {
        if $x > 0{
            print!("\x1b[{}C",($x))
        }
    };
}

use alloc::string::String;
use alloc::vec::Vec;
use user_lib::{
    fork,
    exec,
    waitpid,
    open,
    OpenFlags,
    close,
    dup,
    chdir,
    shutdown,
    ls,
    unlink
};
use user_lib::console::getchar;


const STATE_IDLE:   u8 = 0;
const STATE_PANIC:  u8 = 1;
const STATE_CSI:    u8 = 2;
const STATE_CSI_1:  u8 = 3;
const STATE_CSI_2:  u8 = 4;
pub struct InputMachine{
    state: u8,
    p: usize, // position of input string 
    cmd: String,
    parent_dir_inode_id: usize
}




/// detail of InputMachine is in doc "shell.md"
impl InputMachine{

    pub fn new() -> Self{
        Self{
            state: STATE_IDLE,
            p: 0,
            cmd: String::new(),
            // path: Vec::new(),
            parent_dir_inode_id: 0
        }
    }

    pub fn get_cmd(&mut self) -> &str{
        self.cmd.as_str()
    }

    pub fn clear(&mut self){
        self.state = STATE_IDLE;
        self.p = 0;
        self.cmd.clear();
        // self.path.clear();
        self.parent_dir_inode_id = 0;
    }

    pub fn operate(&mut self, c: char) -> bool{
        // println!("(get char {})",c as u8);
        match self.state {
            STATE_IDLE=>{
                match c as u8 {
                    CSI => {
                        self.state = STATE_CSI;
                    }
                    LF | CR=>{
                        println!("");
                        return true;
                    }
                    BS | DL => {
                        if self.p > 0 {// delete char
                            self.p -= 1;
                            self.cmd.remove(self.p);

                            cursor_move_left!(self.p+1);
                            print!("{} ",self.cmd.as_str());
                            cursor_move_left!(self.cmd.len() - self.p +1);//assert len>=p

                        }
                    }
                    _ =>{
                        if(self.cmd.len() == self.p){
                            self.cmd.insert(self.p, c);
                            print!("{}",c);

                        }
                        else{
                            self.cmd.insert(self.p, c);
                            cursor_move_left!(self.p);
                            
                            // cursor_move_right!(1);
                            print!("{}",self.cmd.as_str());
                            cursor_move_left!(self.cmd.len() - self.p - 1);//assert len>p
                        }
                        self.p += 1;
                    }
                }
            }
            STATE_CSI=>{
                if c == '['{
                    self.state = STATE_CSI_1;
                }
                else{
                    self.state = STATE_PANIC;
                    panic!("Shell input not recognized!(STATE_CSI)");
                }
            }
            STATE_CSI_1=>{
                match c as u8 {
                    51 => {
                        self.state = STATE_CSI_2;
                    }
                    68 => {// KEY LEFT
                        self.state = STATE_IDLE;
                        if self.p > 0 {
                            print!("{}{}{}", CSI as char, '[', c);
                            self.p -= 1;
                        }
                    }
                    67 => {// KEY RIGHT
                        self.state = STATE_IDLE;
                        if self.p < self.cmd.len() {
                            print!("{}{}{}", CSI as char, '[', c);
                            self.p += 1;
                        }
                    }
                    65 | 66 =>{//KEY UP/DOWN not support now
                        self.state = STATE_IDLE;
                    }
                    _ =>{
                        self.state = STATE_PANIC;
                        panic!("Shell input not recognized!(STATE_CSI_1)");
                    }
                }
            }
            STATE_CSI_2=>{
                if c == DEL as char{
                    self.state = STATE_IDLE;
                    self.p -= 1;
                    self.cmd.remove(self.p);
                    cursor_move_left!(self.p+1);
                    print!("{} ",self.cmd.as_str());
                    cursor_move_left!(self.cmd.len() - self.p +1);//assert len>=p
                }
                else{
                    self.state = STATE_PANIC;
                    panic!("Shell input not recognized!(STATE_CSI_2)");
                }
            }
            _ =>{
                panic!("Shell machine state not recognized!");
            }
        }
        return false;
    }
    
    

}

const STATE_ARGS:   u8 = 2;
pub struct ArgMachine{
    args: Vec<String>,
    argc: usize,
    state: u8,
    path: Vec<String>
}

impl ArgMachine{

    fn print_root(&mut self){
        print!("{}@UltraOS: /",color_text!("root",32));
        self.path.iter().for_each(|string|
            print!("{}/", string)
        );
        print!(" >>");
    }

    pub fn new () -> Self{
        let mut new_self = Self{
            args: Vec::new(),
            argc: 0,
            state: 0,
            path: Vec::new()
        };
        new_self.print_root();
        new_self
    }

    // not clear path
    pub fn clear(&mut self){
        self.args.clear();
        self.argc = 0;
        self.state = STATE_IDLE;
        self.print_root();
    }

    pub fn operate(&mut self, c:char){
        match self.state {
            STATE_IDLE=>{
                match c {
                    ' ' => {
                        self.state = STATE_IDLE;
                    }
                    _ =>{//start of an arg
                        self.args.push(String::new());
                        self.args[self.argc].push(c);
                        self.argc += 1;
                        self.state = STATE_ARGS;
                    }
                }
            }
            STATE_ARGS=>{
                match c {
                    ' ' => {// end of one arg
                        self.args[self.argc-1].push(0 as char);
                        self.state = STATE_IDLE;
                    }
                    _ =>{
                        self.args[self.argc-1].push(c);
                        self.state = STATE_ARGS;
                    }
                }
            }
            _ =>{
                panic!("Arg machine state not recognized!");
            }
        }
    }

    // @return: true -> valid "exec" argc (op "cd" not included)
    pub fn operate_str(&mut self, str: &str) -> bool {
        for c in str.chars() {
            self.operate(c);
        }
        if self.state == STATE_ARGS{
            self.args[self.argc-1].push(0 as char);
        }
        // self.print_state();
        // \0 indicates the end of str while rust doesn't do so
        if self.args.is_empty(){
            return false;
        }
        // cd
        if self.args[0].clone().as_str() == "cd\0" {
            if self.argc != 2 {
                println!("cd: Arg expression not right, should be: cd <addr>");
            }
            else{
                self.change_dir();
            }
            return false;
        }
        // print!("changed!");
        // autorun
        if self.args[0].clone().as_str() == "run_testsuites\0" {
            let mut testsuits :Vec<&str>= Vec::new();
            testsuits.push("testsuites_times\0");
            testsuits.push("testsuites_gettimeofday\0");
            testsuits.push("testsuites_sleep\0");
            testsuits.push("testsuites_brk\0");
            testsuits.push("testsuites_clone\0");
            testsuits.push("testsuites_close\0");
            testsuits.push("testsuites_dup2\0");
            testsuits.push("testsuites_dup\0");
            testsuits.push("testsuites_execve\0");
            testsuits.push("testsuites_exit\0");
            testsuits.push("testsuites_fork\0");
            testsuits.push("testsuites_fstat\0");
            testsuits.push("testsuites_getcwd\0");
            testsuits.push("testsuites_getdents\0");
            testsuits.push("testsuites_getpid\0");
            testsuits.push("testsuites_getppid\0");
            // testsuits.push("testsuites_mkdir_\0");
            // testsuits.push("testsuites_mmap\0");
            // testsuits.push("testsuites_munmap\0");
            testsuits.push("testsuites_mount\0");
            testsuits.push("testsuites_openat\0");
            testsuits.push("testsuites_open\0");
            testsuits.push("testsuites_pipe\0");
            testsuits.push("testsuites_read\0");
            testsuits.push("testsuites_umount\0");
            testsuits.push("testsuites_uname\0");
            testsuits.push("testsuites_wait\0");
            testsuits.push("testsuites_waitpid\0");
            testsuits.push("testsuites_write\0");
            testsuits.push("testsuites_yield\0");
            testsuits.push("testsuites_unlink\0");
            testsuits.push("testsuites_chdir\0");
            if self.argc != 1 {
                println!("cd: Arg expression not right, should be: run_testsuits");
            }
            else{
                for programname in testsuits.iter(){
                    let pid = fork();
                    let mut exit_code = 0;
                    let mut args_addr: Vec<*const u8> = Vec::new();
                    args_addr.push(0 as *const u8);
                    if pid == 0 {
                        // child process
                        if exec(programname, args_addr.as_slice()) == -1 {
                            println!("Error when executing run_testsuites!");
                            return false;
                        }
                        unreachable!();
                    } else {
                        waitpid(pid as usize, &mut exit_code);
                    }
                }
            }
            return false;
        }
        // Standard autorun
        if self.args[0].clone().as_str() == "autorun\0" {
            ArgMachine::auto_run_testsuites();
            return false
        }
        // ls
        if self.args[0].clone().as_str() == "ls\0" {
            // println!("(into ls)");
            if !(self.argc == 2 || self.argc == 1) {
                println!("ls: Arg expression not right");
                return false
            }
            
            if self.argc == 1 {
                ls("\0");
            } else{
                ls(self.args[1].clone().as_str());
            }
            return false
        }
        return true;
    }

    pub fn auto_run_testsuites(){
        // ls("\0");
        let mut testsuits :Vec<&str>= Vec::new();
        testsuits.push("brk\0");
        testsuits.push("chdir\0");
        testsuits.push("clone\0");
        testsuits.push("close\0");
        testsuits.push("dup2\0");
        testsuits.push("dup\0");
        testsuits.push("execve\0");
        testsuits.push("exit\0");
        testsuits.push("fork\0");
        testsuits.push("fstat\0");
        testsuits.push("getcwd\0");
        testsuits.push("getdents\0");
        testsuits.push("getpid\0");
        testsuits.push("getppid\0");
        testsuits.push("mkdir_\0");
        testsuits.push("mmap\0");
        testsuits.push("mount\0");
        testsuits.push("munmap\0");
        testsuits.push("openat\0");
        testsuits.push("open\0");
        testsuits.push("pipe\0");
        testsuits.push("read\0");
        testsuits.push("umount\0");
        testsuits.push("uname\0");
        testsuits.push("wait\0");
        testsuits.push("waitpid\0");
        testsuits.push("write\0");
        testsuits.push("yield\0");
        testsuits.push("sleep\0");
        testsuits.push("times\0");
        testsuits.push("gettimeofday\0");
        testsuits.push("unlink\0");
        for programname in testsuits.iter(){
            let mut exit_code = 0;
            let mut args_addr: Vec<*const u8> = Vec::new();
            args_addr.push(0 as *const u8);
            let pid = fork();
            if pid == 0 {
                // child process
                //println!("{}",programname);
                //if *programname == "mkdir_\0" {
                //    ls("\0");
                //}
                if exec(programname, args_addr.as_slice()) == -1 {
                    println!("Error when executing autorun_testsuites!");
                    shutdown();
                }
                unreachable!();
            } else {
                waitpid(pid as usize, &mut exit_code);
            }
        }
        shutdown();
    }

    pub fn print_state(&mut self){
        println!("argc: {}\nstate: {}",
        self.argc,self.state);
        for i in 0..self.argc{
            println!("arg[{}]:{}",i, self.args[i].clone().as_str());
        } 
    }

    pub fn get_args(&mut self) -> (Vec<String>, String, String){
        //copy args
        let mut args_copy: Vec<String> = Vec::new();
        self.args
            .iter_mut()
            .for_each(|string| {
                args_copy.push(string.clone());
            });

        // redirect input
        let mut input = String::new();
        if let Some((idx, _)) = args_copy
        .iter()
        .enumerate()
        .find(|(_, arg)| arg.as_str() == "<\0") {
            input = args_copy[idx + 1].clone();
            args_copy.drain(idx..=idx + 1);
        }

        // redirect output
        let mut output = String::new();
        if let Some((idx, _)) = args_copy
        .iter()
        .enumerate()
        .find(|(_, arg)| arg.as_str() == ">\0") {
            output = args_copy[idx + 1].clone();
            args_copy.drain(idx..=idx + 1);
        };

        (args_copy, input, output)
    }

    // Assume "self.args' meet "cd" requirements
    pub fn change_dir(&mut self){
        let mut cd_path = self.args[1].clone();
        if chdir(cd_path.as_str()) == -1{
            println!("cd: No such directory!");
            return;
        }

        cd_path.pop();// clear '\0' at the end of str
        let mut cd_path_vec:Vec<String> = Vec::new();
        cd_path.as_str().split('/').for_each(
            |str| cd_path_vec.push(String::from(str))
        );

        let is_from_root = cd_path_vec[0].is_empty(); // start from '/' root
        if is_from_root {
            self.path.clear();
        }
        cd_path_vec.iter().for_each(
            |string| // name of every single directory entry(eg. /hello/world -> "hello","world")
            if !string.is_empty() && string.as_str() != "."{
                if string.as_str() == ".."{
                    self.path.pop();
                }
                else{
                    self.path.push(string.clone());
                }
            }
        );
    }

    
}

#[no_mangle]
pub fn main() -> i32 {
    // delete init programs in fs
    unlink("initproc\0");
    unlink("user_shell\0");
    println!("Delete init programs initproc and user_shell in FS");
    ArgMachine::auto_run_testsuites();
    let mut line: String;
    let left = 3;
    // cursor_move_right!(left);
    // cursor_move_right!(3);
    // println!("{}","TTTTTTTTTTTTTTTTTTEST  test start");
    // print!("{}{}{}","hello", " ", "delete");
    // cursor_move_left!(3);
    // cursor_move_left!(left);
    // print!("{}{}K",CSI as char, 2u8 as char);
    // print!("\r");
    // println!("{}{}{}","hello", KEY_LEFT, "left");
    // println!("{}{}{}","hello", KEY_RIGHT, "fuck");
    // println!("{}{}{}","hello", KEY_DOWN, "down");
    // println!("{}{}{}","hello", KEY_UP, "up");
    // println!("sth_in:{}",sth_in);
    // let mut sth = 3;
    // println!("{}","TTTTTTTTTTTTTTTTTTEST  test end");
    let mut shellmachine = InputMachine::new();
    let mut arg_machine = ArgMachine::new();
    loop {
        let c = getchar();
        let is_exec = shellmachine.operate(c as char);
        if is_exec {
            line = String::from(shellmachine.get_cmd());
            let is_exec = arg_machine.operate_str(shellmachine.get_cmd());
            // arg_machine.print_state();
            if line.is_empty() || !is_exec{
                shellmachine.clear();
                arg_machine.clear();
                continue;
            }
            // println!("Input:{}",line);
            let (args_copy,input,output) = arg_machine.get_args();
            
            let mut args_addr: Vec<*const u8> = args_copy
            .iter()
            .map(|arg| arg.as_ptr())
            .collect();
            args_addr.push(0 as *const u8);
            let pid = fork();
            if pid == 0 {
                // input redirection
                if !input.is_empty() {
                    let input_fd = open(input.as_str(), OpenFlags::RDONLY);
                    if input_fd == -1 {
                        println!("Error when opening file {}", input);
                        return -4;
                    }
                    let input_fd = input_fd as usize;
                    close(0);
                    assert_eq!(dup(input_fd), 0);
                    close(input_fd);
                }
                // output redirection
                if !output.is_empty() {
                    let output_fd = open(
                        output.as_str(),
                        OpenFlags::CREATE | OpenFlags::WRONLY
                    );
                    if output_fd == -1 {
                        println!("Error when opening file {}", output);
                        return -4;
                    }
                    let output_fd = output_fd as usize;
                    close(1);
                    assert_eq!(dup(output_fd), 1);
                    close(output_fd);
                }
                // child process
                if exec(args_copy[0].as_str(), args_addr.as_slice()) == -1 {
                    println!("Error when executing!");
                    return -4;
                }
                unreachable!();
            } else {
                let mut exit_code: i32 = 0;
                let exit_pid = waitpid(pid as usize, &mut exit_code);
                assert_eq!(pid, exit_pid);
                println!("Shell: Process {} exited with code {}", pid, exit_code);
                shellmachine.clear();
                arg_machine.clear();
            }
        }
    }
}