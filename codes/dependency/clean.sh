for programname in $(ls)
do 
    if test -d $programname
    then 
        # cd $programname 
        git rm -rf --cached $programname/
        # ls
        # cd ..
    fi
    # cd $programname && sudo rm -r .github && sudo rm -r .gitignore
done