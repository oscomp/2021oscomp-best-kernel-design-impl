/********************************************************************************************************* 
** 													
**                                    中国软件开源组织					
**														
**                                   嵌入式实时操作系统				
**														
**                                       SylixOS(TM)	
**														
**                               Copyright  All Rights Reserved		
**														
**--------------文件信息--------------------------------------------------------------------------------	
**														
** 文   件   名: symbol.c								
**														
** 创   建   人: makesymbol 工具						
**														
** 文件创建日期: 2021 年 05 月 18 日			
**														
** 描        述: 系统 sylixos 符号表. (此文件由 makesymbol 工具自动生成, 请勿修改)	
*********************************************************************************************************/	
#ifdef __GNUC__                                        
#if __GNUC__ <= 4                                    
#pragma GCC diagnostic warning "-w"                    
#elif __GNUC__ >= 7                                  
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch" 
#endif                                                 
#endif                                                 
														
#include "symboltools.h"								
														
#define SYMBOL_TABLE_BEGIN LW_STATIC_SYMBOL   _G_symLibSylixOS[] = {	
  														
#define SYMBOL_TABLE_END };							
														
#define SYMBOL_ITEM_FUNC(pcName)                       \
    {   {(void *)0, (void *)0},                        \
        #pcName, (char *)pcName,                       \
        LW_SYMBOL_TEXT                                 \
    },													
														
#define SYMBOL_ITEM_OBJ(pcName)                       \
    {   {(void *)0, (void *)0},                       \
        #pcName, (char *)&pcName,                     \
        LW_SYMBOL_DATA                                \
    },													
														
/*********************************************************************************************************	
  全局对象声明											
*********************************************************************************************************/	
#ifdef SYLIXOS_EXPORT_KSYMBOL							
extern int  bb_putchar(); 
extern int  full_write(); 
extern int  get_terminal_width_height(); 
extern int  last_char_is(); 
extern int  lib_xzalloc(); 
extern int  vi_safe_poll(); 
extern int  vi_safe_read(); 
extern int  vi_safe_write(); 
extern int  viShellInit(); 
extern int  vi_main(); 
extern int  mbedtls_aes_crypt_cbc(); 
extern int  mbedtls_aes_crypt_cfb128(); 
extern int  mbedtls_aes_crypt_cfb8(); 
extern int  mbedtls_aes_crypt_ctr(); 
extern int  mbedtls_aes_crypt_ecb(); 
extern int  mbedtls_aes_decrypt(); 
extern int  mbedtls_aes_encrypt(); 
extern int  mbedtls_aes_free(); 
extern int  mbedtls_aes_init(); 
extern int  mbedtls_aes_self_test(); 
extern int  mbedtls_aes_setkey_dec(); 
extern int  mbedtls_aes_setkey_enc(); 
extern int  mbedtls_internal_aes_decrypt(); 
extern int  mbedtls_internal_aes_encrypt(); 
extern int  mbedtls_arc4_crypt(); 
extern int  mbedtls_arc4_free(); 
extern int  mbedtls_arc4_init(); 
extern int  mbedtls_arc4_self_test(); 
extern int  mbedtls_arc4_setup(); 
extern int  mbedtls_asn1_find_named_data(); 
extern int  mbedtls_asn1_free_named_data(); 
extern int  mbedtls_asn1_free_named_data_list(); 
extern int  mbedtls_asn1_get_alg(); 
extern int  mbedtls_asn1_get_alg_null(); 
extern int  mbedtls_asn1_get_bitstring(); 
extern int  mbedtls_asn1_get_bitstring_null(); 
extern int  mbedtls_asn1_get_bool(); 
extern int  mbedtls_asn1_get_int(); 
extern int  mbedtls_asn1_get_len(); 
extern int  mbedtls_asn1_get_mpi(); 
extern int  mbedtls_asn1_get_sequence_of(); 
extern int  mbedtls_asn1_get_tag(); 
extern int  mbedtls_asn1_store_named_data(); 
extern int  mbedtls_asn1_write_algorithm_identifier(); 
extern int  mbedtls_asn1_write_bitstring(); 
extern int  mbedtls_asn1_write_bool(); 
extern int  mbedtls_asn1_write_ia5_string(); 
extern int  mbedtls_asn1_write_int(); 
extern int  mbedtls_asn1_write_len(); 
extern int  mbedtls_asn1_write_mpi(); 
extern int  mbedtls_asn1_write_null(); 
extern int  mbedtls_asn1_write_octet_string(); 
extern int  mbedtls_asn1_write_oid(); 
extern int  mbedtls_asn1_write_printable_string(); 
extern int  mbedtls_asn1_write_raw_buffer(); 
extern int  mbedtls_asn1_write_tag(); 
extern int  mbedtls_base64_decode(); 
extern int  mbedtls_base64_encode(); 
extern int  mbedtls_base64_self_test(); 
extern int  mbedtls_mpi_add_abs(); 
extern int  mbedtls_mpi_add_int(); 
extern int  mbedtls_mpi_add_mpi(); 
extern int  mbedtls_mpi_bitlen(); 
extern int  mbedtls_mpi_cmp_abs(); 
extern int  mbedtls_mpi_cmp_int(); 
extern int  mbedtls_mpi_cmp_mpi(); 
extern int  mbedtls_mpi_copy(); 
extern int  mbedtls_mpi_div_int(); 
extern int  mbedtls_mpi_div_mpi(); 
extern int  mbedtls_mpi_exp_mod(); 
extern int  mbedtls_mpi_fill_random(); 
extern int  mbedtls_mpi_free(); 
extern int  mbedtls_mpi_gcd(); 
extern int  mbedtls_mpi_gen_prime(); 
extern int  mbedtls_mpi_get_bit(); 
extern int  mbedtls_mpi_grow(); 
extern int  mbedtls_mpi_init(); 
extern int  mbedtls_mpi_inv_mod(); 
extern int  mbedtls_mpi_is_prime(); 
extern int  mbedtls_mpi_lsb(); 
extern int  mbedtls_mpi_lset(); 
extern int  mbedtls_mpi_mod_int(); 
extern int  mbedtls_mpi_mod_mpi(); 
extern int  mbedtls_mpi_mul_int(); 
extern int  mbedtls_mpi_mul_mpi(); 
extern int  mbedtls_mpi_read_binary(); 
extern int  mbedtls_mpi_read_file(); 
extern int  mbedtls_mpi_read_string(); 
extern int  mbedtls_mpi_safe_cond_assign(); 
extern int  mbedtls_mpi_safe_cond_swap(); 
extern int  mbedtls_mpi_self_test(); 
extern int  mbedtls_mpi_set_bit(); 
extern int  mbedtls_mpi_shift_l(); 
extern int  mbedtls_mpi_shift_r(); 
extern int  mbedtls_mpi_shrink(); 
extern int  mbedtls_mpi_size(); 
extern int  mbedtls_mpi_sub_abs(); 
extern int  mbedtls_mpi_sub_int(); 
extern int  mbedtls_mpi_sub_mpi(); 
extern int  mbedtls_mpi_swap(); 
extern int  mbedtls_mpi_write_binary(); 
extern int  mbedtls_mpi_write_file(); 
extern int  mbedtls_mpi_write_string(); 
extern int  mbedtls_blowfish_crypt_cbc(); 
extern int  mbedtls_blowfish_crypt_cfb64(); 
extern int  mbedtls_blowfish_crypt_ctr(); 
extern int  mbedtls_blowfish_crypt_ecb(); 
extern int  mbedtls_blowfish_free(); 
extern int  mbedtls_blowfish_init(); 
extern int  mbedtls_blowfish_setkey(); 
extern int  mbedtls_camellia_crypt_cbc(); 
extern int  mbedtls_camellia_crypt_cfb128(); 
extern int  mbedtls_camellia_crypt_ctr(); 
extern int  mbedtls_camellia_crypt_ecb(); 
extern int  mbedtls_camellia_free(); 
extern int  mbedtls_camellia_init(); 
extern int  mbedtls_camellia_self_test(); 
extern int  mbedtls_camellia_setkey_dec(); 
extern int  mbedtls_camellia_setkey_enc(); 
extern int  mbedtls_ccm_auth_decrypt(); 
extern int  mbedtls_ccm_encrypt_and_tag(); 
extern int  mbedtls_ccm_free(); 
extern int  mbedtls_ccm_init(); 
extern int  mbedtls_ccm_self_test(); 
extern int  mbedtls_ccm_setkey(); 
extern int  mbedtls_cipher_auth_decrypt(); 
extern int  mbedtls_cipher_auth_encrypt(); 
extern int  mbedtls_cipher_check_tag(); 
extern int  mbedtls_cipher_crypt(); 
extern int  mbedtls_cipher_finish(); 
extern int  mbedtls_cipher_free(); 
extern int  mbedtls_cipher_info_from_string(); 
extern int  mbedtls_cipher_info_from_type(); 
extern int  mbedtls_cipher_info_from_values(); 
extern int  mbedtls_cipher_init(); 
extern int  mbedtls_cipher_list(); 
extern int  mbedtls_cipher_reset(); 
extern int  mbedtls_cipher_set_iv(); 
extern int  mbedtls_cipher_set_padding_mode(); 
extern int  mbedtls_cipher_setkey(); 
extern int  mbedtls_cipher_setup(); 
extern int  mbedtls_cipher_update(); 
extern int  mbedtls_cipher_update_ad(); 
extern int  mbedtls_cipher_write_tag(); 
extern int  mbedtls_ctr_drbg_free(); 
extern int  mbedtls_ctr_drbg_init(); 
extern int  mbedtls_ctr_drbg_random(); 
extern int  mbedtls_ctr_drbg_random_with_add(); 
extern int  mbedtls_ctr_drbg_reseed(); 
extern int  mbedtls_ctr_drbg_seed(); 
extern int  mbedtls_ctr_drbg_seed_entropy_len(); 
extern int  mbedtls_ctr_drbg_self_test(); 
extern int  mbedtls_ctr_drbg_set_entropy_len(); 
extern int  mbedtls_ctr_drbg_set_prediction_resistance(); 
extern int  mbedtls_ctr_drbg_set_reseed_interval(); 
extern int  mbedtls_ctr_drbg_update(); 
extern int  mbedtls_ctr_drbg_update_seed_file(); 
extern int  mbedtls_ctr_drbg_write_seed_file(); 
extern int  mbedtls_debug_print_buf(); 
extern int  mbedtls_debug_print_crt(); 
extern int  mbedtls_debug_print_ecp(); 
extern int  mbedtls_debug_print_mpi(); 
extern int  mbedtls_debug_print_msg(); 
extern int  mbedtls_debug_print_ret(); 
extern int  mbedtls_debug_set_threshold(); 
extern int  mbedtls_des_crypt_cbc(); 
extern int  mbedtls_des_crypt_ecb(); 
extern int  mbedtls_des_free(); 
extern int  mbedtls_des_init(); 
extern int  mbedtls_des_key_check_key_parity(); 
extern int  mbedtls_des_key_check_weak(); 
extern int  mbedtls_des_key_set_parity(); 
extern int  mbedtls_des_self_test(); 
extern int  mbedtls_des_setkey(); 
extern int  mbedtls_des_setkey_dec(); 
extern int  mbedtls_des_setkey_enc(); 
extern int  mbedtls_des3_crypt_cbc(); 
extern int  mbedtls_des3_crypt_ecb(); 
extern int  mbedtls_des3_free(); 
extern int  mbedtls_des3_init(); 
extern int  mbedtls_des3_set2key_dec(); 
extern int  mbedtls_des3_set2key_enc(); 
extern int  mbedtls_des3_set3key_dec(); 
extern int  mbedtls_des3_set3key_enc(); 
extern int  mbedtls_dhm_calc_secret(); 
extern int  mbedtls_dhm_free(); 
extern int  mbedtls_dhm_init(); 
extern int  mbedtls_dhm_make_params(); 
extern int  mbedtls_dhm_make_public(); 
extern int  mbedtls_dhm_parse_dhm(); 
extern int  mbedtls_dhm_parse_dhmfile(); 
extern int  mbedtls_dhm_read_params(); 
extern int  mbedtls_dhm_read_public(); 
extern int  mbedtls_dhm_self_test(); 
extern int  mbedtls_ecdh_calc_secret(); 
extern int  mbedtls_ecdh_compute_shared(); 
extern int  mbedtls_ecdh_free(); 
extern int  mbedtls_ecdh_gen_public(); 
extern int  mbedtls_ecdh_get_params(); 
extern int  mbedtls_ecdh_init(); 
extern int  mbedtls_ecdh_make_params(); 
extern int  mbedtls_ecdh_make_public(); 
extern int  mbedtls_ecdh_read_params(); 
extern int  mbedtls_ecdh_read_public(); 
extern int  mbedtls_ecdsa_free(); 
extern int  mbedtls_ecdsa_from_keypair(); 
extern int  mbedtls_ecdsa_genkey(); 
extern int  mbedtls_ecdsa_init(); 
extern int  mbedtls_ecdsa_read_signature(); 
extern int  mbedtls_ecdsa_sign(); 
extern int  mbedtls_ecdsa_sign_det(); 
extern int  mbedtls_ecdsa_verify(); 
extern int  mbedtls_ecdsa_write_signature(); 
extern int  mbedtls_ecdsa_write_signature_det(); 
extern int  mbedtls_ecp_group_load(); 
extern int  mbedtls_ecp_check_privkey(); 
extern int  mbedtls_ecp_check_pub_priv(); 
extern int  mbedtls_ecp_check_pubkey(); 
extern int  mbedtls_ecp_copy(); 
extern int  mbedtls_ecp_curve_info_from_grp_id(); 
extern int  mbedtls_ecp_curve_info_from_name(); 
extern int  mbedtls_ecp_curve_info_from_tls_id(); 
extern int  mbedtls_ecp_curve_list(); 
extern int  mbedtls_ecp_gen_key(); 
extern int  mbedtls_ecp_gen_keypair(); 
extern int  mbedtls_ecp_gen_keypair_base(); 
extern int  mbedtls_ecp_group_copy(); 
extern int  mbedtls_ecp_group_free(); 
extern int  mbedtls_ecp_group_init(); 
extern int  mbedtls_ecp_grp_id_list(); 
extern int  mbedtls_ecp_is_zero(); 
extern int  mbedtls_ecp_keypair_free(); 
extern int  mbedtls_ecp_keypair_init(); 
extern int  mbedtls_ecp_mul(); 
extern int  mbedtls_ecp_muladd(); 
extern int  mbedtls_ecp_point_cmp(); 
extern int  mbedtls_ecp_point_free(); 
extern int  mbedtls_ecp_point_init(); 
extern int  mbedtls_ecp_point_read_binary(); 
extern int  mbedtls_ecp_point_read_string(); 
extern int  mbedtls_ecp_point_write_binary(); 
extern int  mbedtls_ecp_self_test(); 
extern int  mbedtls_ecp_set_zero(); 
extern int  mbedtls_ecp_tls_read_group(); 
extern int  mbedtls_ecp_tls_read_point(); 
extern int  mbedtls_ecp_tls_write_group(); 
extern int  mbedtls_ecp_tls_write_point(); 
extern int  mbedtls_entropy_add_source(); 
extern int  mbedtls_entropy_free(); 
extern int  mbedtls_entropy_func(); 
extern int  mbedtls_entropy_gather(); 
extern int  mbedtls_entropy_init(); 
extern int  mbedtls_entropy_self_test(); 
extern int  mbedtls_entropy_update_manual(); 
extern int  mbedtls_entropy_update_seed_file(); 
extern int  mbedtls_entropy_write_seed_file(); 
extern int  mbedtls_hardclock_poll(); 
extern int  mbedtls_platform_entropy_poll(); 
extern int  mbedtls_strerror(); 
extern int  mbedtls_gcm_auth_decrypt(); 
extern int  mbedtls_gcm_crypt_and_tag(); 
extern int  mbedtls_gcm_finish(); 
extern int  mbedtls_gcm_free(); 
extern int  mbedtls_gcm_init(); 
extern int  mbedtls_gcm_self_test(); 
extern int  mbedtls_gcm_setkey(); 
extern int  mbedtls_gcm_starts(); 
extern int  mbedtls_gcm_update(); 
extern int  mbedtls_hmac_drbg_free(); 
extern int  mbedtls_hmac_drbg_init(); 
extern int  mbedtls_hmac_drbg_random(); 
extern int  mbedtls_hmac_drbg_random_with_add(); 
extern int  mbedtls_hmac_drbg_reseed(); 
extern int  mbedtls_hmac_drbg_seed(); 
extern int  mbedtls_hmac_drbg_seed_buf(); 
extern int  mbedtls_hmac_drbg_self_test(); 
extern int  mbedtls_hmac_drbg_set_entropy_len(); 
extern int  mbedtls_hmac_drbg_set_prediction_resistance(); 
extern int  mbedtls_hmac_drbg_set_reseed_interval(); 
extern int  mbedtls_hmac_drbg_update(); 
extern int  mbedtls_hmac_drbg_update_seed_file(); 
extern int  mbedtls_hmac_drbg_write_seed_file(); 
extern int  mbedtls_md(); 
extern int  mbedtls_md_clone(); 
extern int  mbedtls_md_file(); 
extern int  mbedtls_md_finish(); 
extern int  mbedtls_md_free(); 
extern int  mbedtls_md_get_name(); 
extern int  mbedtls_md_get_size(); 
extern int  mbedtls_md_get_type(); 
extern int  mbedtls_md_hmac(); 
extern int  mbedtls_md_hmac_finish(); 
extern int  mbedtls_md_hmac_reset(); 
extern int  mbedtls_md_hmac_starts(); 
extern int  mbedtls_md_hmac_update(); 
extern int  mbedtls_md_info_from_string(); 
extern int  mbedtls_md_info_from_type(); 
extern int  mbedtls_md_init(); 
extern int  mbedtls_md_init_ctx(); 
extern int  mbedtls_md_list(); 
extern int  mbedtls_md_process(); 
extern int  mbedtls_md_setup(); 
extern int  mbedtls_md_starts(); 
extern int  mbedtls_md_update(); 
extern int  mbedtls_md2(); 
extern int  mbedtls_md2_clone(); 
extern int  mbedtls_md2_finish(); 
extern int  mbedtls_md2_free(); 
extern int  mbedtls_md2_init(); 
extern int  mbedtls_md2_process(); 
extern int  mbedtls_md2_self_test(); 
extern int  mbedtls_md2_starts(); 
extern int  mbedtls_md2_update(); 
extern int  mbedtls_md4(); 
extern int  mbedtls_md4_clone(); 
extern int  mbedtls_md4_finish(); 
extern int  mbedtls_md4_free(); 
extern int  mbedtls_md4_init(); 
extern int  mbedtls_md4_process(); 
extern int  mbedtls_md4_self_test(); 
extern int  mbedtls_md4_starts(); 
extern int  mbedtls_md4_update(); 
extern int  mbedtls_md5(); 
extern int  mbedtls_md5_clone(); 
extern int  mbedtls_md5_finish(); 
extern int  mbedtls_md5_free(); 
extern int  mbedtls_md5_init(); 
extern int  mbedtls_md5_process(); 
extern int  mbedtls_md5_self_test(); 
extern int  mbedtls_md5_starts(); 
extern int  mbedtls_md5_update(); 
extern int  mbedtls_net_accept(); 
extern int  mbedtls_net_bind(); 
extern int  mbedtls_net_connect(); 
extern int  mbedtls_net_free(); 
extern int  mbedtls_net_init(); 
extern int  mbedtls_net_recv(); 
extern int  mbedtls_net_recv_timeout(); 
extern int  mbedtls_net_send(); 
extern int  mbedtls_net_set_block(); 
extern int  mbedtls_net_set_nonblock(); 
extern int  mbedtls_net_usleep(); 
extern int  mbedtls_oid_get_attr_short_name(); 
extern int  mbedtls_oid_get_cipher_alg(); 
extern int  mbedtls_oid_get_ec_grp(); 
extern int  mbedtls_oid_get_extended_key_usage(); 
extern int  mbedtls_oid_get_md_alg(); 
extern int  mbedtls_oid_get_numeric_string(); 
extern int  mbedtls_oid_get_oid_by_ec_grp(); 
extern int  mbedtls_oid_get_oid_by_md(); 
extern int  mbedtls_oid_get_oid_by_pk_alg(); 
extern int  mbedtls_oid_get_oid_by_sig_alg(); 
extern int  mbedtls_oid_get_pk_alg(); 
extern int  mbedtls_oid_get_pkcs12_pbe_alg(); 
extern int  mbedtls_oid_get_sig_alg(); 
extern int  mbedtls_oid_get_sig_alg_desc(); 
extern int  mbedtls_oid_get_x509_ext_type(); 
extern int  mbedtls_pem_free(); 
extern int  mbedtls_pem_init(); 
extern int  mbedtls_pem_read_buffer(); 
extern int  mbedtls_pem_write_buffer(); 
extern int  mbedtls_pk_can_do(); 
extern int  mbedtls_pk_check_pair(); 
extern int  mbedtls_pk_debug(); 
extern int  mbedtls_pk_decrypt(); 
extern int  mbedtls_pk_encrypt(); 
extern int  mbedtls_pk_free(); 
extern int  mbedtls_pk_get_bitlen(); 
extern int  mbedtls_pk_get_name(); 
extern int  mbedtls_pk_get_type(); 
extern int  mbedtls_pk_info_from_type(); 
extern int  mbedtls_pk_init(); 
extern int  mbedtls_pk_setup(); 
extern int  mbedtls_pk_setup_rsa_alt(); 
extern int  mbedtls_pk_sign(); 
extern int  mbedtls_pk_verify(); 
extern int  mbedtls_pk_verify_ext(); 
extern int  mbedtls_pkcs12_derivation(); 
extern int  mbedtls_pkcs12_pbe(); 
extern int  mbedtls_pkcs12_pbe_sha1_rc4_128(); 
extern int  mbedtls_pkcs5_pbes2(); 
extern int  mbedtls_pkcs5_pbkdf2_hmac(); 
extern int  mbedtls_pkcs5_self_test(); 
extern int  mbedtls_pk_load_file(); 
extern int  mbedtls_pk_parse_key(); 
extern int  mbedtls_pk_parse_keyfile(); 
extern int  mbedtls_pk_parse_public_key(); 
extern int  mbedtls_pk_parse_public_keyfile(); 
extern int  mbedtls_pk_parse_subpubkey(); 
extern int  mbedtls_pk_write_key_der(); 
extern int  mbedtls_pk_write_key_pem(); 
extern int  mbedtls_pk_write_pubkey(); 
extern int  mbedtls_pk_write_pubkey_der(); 
extern int  mbedtls_pk_write_pubkey_pem(); 
extern int  mbedtls_platform_setup(); 
extern int  mbedtls_platform_teardown(); 
extern int  mbedtls_ripemd160(); 
extern int  mbedtls_ripemd160_clone(); 
extern int  mbedtls_ripemd160_finish(); 
extern int  mbedtls_ripemd160_free(); 
extern int  mbedtls_ripemd160_init(); 
extern int  mbedtls_ripemd160_process(); 
extern int  mbedtls_ripemd160_self_test(); 
extern int  mbedtls_ripemd160_starts(); 
extern int  mbedtls_ripemd160_update(); 
extern int  mbedtls_rsa_check_privkey(); 
extern int  mbedtls_rsa_check_pub_priv(); 
extern int  mbedtls_rsa_check_pubkey(); 
extern int  mbedtls_rsa_copy(); 
extern int  mbedtls_rsa_free(); 
extern int  mbedtls_rsa_gen_key(); 
extern int  mbedtls_rsa_init(); 
extern int  mbedtls_rsa_pkcs1_decrypt(); 
extern int  mbedtls_rsa_pkcs1_encrypt(); 
extern int  mbedtls_rsa_pkcs1_sign(); 
extern int  mbedtls_rsa_pkcs1_verify(); 
extern int  mbedtls_rsa_private(); 
extern int  mbedtls_rsa_public(); 
extern int  mbedtls_rsa_rsaes_oaep_decrypt(); 
extern int  mbedtls_rsa_rsaes_oaep_encrypt(); 
extern int  mbedtls_rsa_rsaes_pkcs1_v15_decrypt(); 
extern int  mbedtls_rsa_rsaes_pkcs1_v15_encrypt(); 
extern int  mbedtls_rsa_rsassa_pkcs1_v15_sign(); 
extern int  mbedtls_rsa_rsassa_pkcs1_v15_verify(); 
extern int  mbedtls_rsa_rsassa_pss_sign(); 
extern int  mbedtls_rsa_rsassa_pss_verify(); 
extern int  mbedtls_rsa_rsassa_pss_verify_ext(); 
extern int  mbedtls_rsa_self_test(); 
extern int  mbedtls_rsa_set_padding(); 
extern int  mbedtls_sha1(); 
extern int  mbedtls_sha1_clone(); 
extern int  mbedtls_sha1_finish(); 
extern int  mbedtls_sha1_free(); 
extern int  mbedtls_sha1_init(); 
extern int  mbedtls_sha1_process(); 
extern int  mbedtls_sha1_self_test(); 
extern int  mbedtls_sha1_starts(); 
extern int  mbedtls_sha1_update(); 
extern int  mbedtls_sha256(); 
extern int  mbedtls_sha256_clone(); 
extern int  mbedtls_sha256_finish(); 
extern int  mbedtls_sha256_free(); 
extern int  mbedtls_sha256_init(); 
extern int  mbedtls_sha256_process(); 
extern int  mbedtls_sha256_self_test(); 
extern int  mbedtls_sha256_starts(); 
extern int  mbedtls_sha256_update(); 
extern int  mbedtls_sha512(); 
extern int  mbedtls_sha512_clone(); 
extern int  mbedtls_sha512_finish(); 
extern int  mbedtls_sha512_free(); 
extern int  mbedtls_sha512_init(); 
extern int  mbedtls_sha512_process(); 
extern int  mbedtls_sha512_self_test(); 
extern int  mbedtls_sha512_starts(); 
extern int  mbedtls_sha512_update(); 
extern int  mbedtls_ssl_cache_free(); 
extern int  mbedtls_ssl_cache_get(); 
extern int  mbedtls_ssl_cache_init(); 
extern int  mbedtls_ssl_cache_set(); 
extern int  mbedtls_ssl_cache_set_max_entries(); 
extern int  mbedtls_ssl_cache_set_timeout(); 
extern int  mbedtls_ssl_ciphersuite_from_id(); 
extern int  mbedtls_ssl_ciphersuite_from_string(); 
extern int  mbedtls_ssl_ciphersuite_uses_ec(); 
extern int  mbedtls_ssl_ciphersuite_uses_psk(); 
extern int  mbedtls_ssl_get_ciphersuite_id(); 
extern int  mbedtls_ssl_get_ciphersuite_name(); 
extern int  mbedtls_ssl_get_ciphersuite_sig_alg(); 
extern int  mbedtls_ssl_get_ciphersuite_sig_pk_alg(); 
extern int  mbedtls_ssl_list_ciphersuites(); 
extern int  mbedtls_ssl_handshake_client_step(); 
extern int  mbedtls_ssl_cookie_check(); 
extern int  mbedtls_ssl_cookie_free(); 
extern int  mbedtls_ssl_cookie_init(); 
extern int  mbedtls_ssl_cookie_set_timeout(); 
extern int  mbedtls_ssl_cookie_setup(); 
extern int  mbedtls_ssl_cookie_write(); 
extern int  mbedtls_ssl_conf_dtls_cookies(); 
extern int  mbedtls_ssl_handshake_server_step(); 
extern int  mbedtls_ssl_set_client_transport_id(); 
extern int  mbedtls_ssl_ticket_free(); 
extern int  mbedtls_ssl_ticket_init(); 
extern int  mbedtls_ssl_ticket_parse(); 
extern int  mbedtls_ssl_ticket_setup(); 
extern int  mbedtls_ssl_ticket_write(); 
extern int  mbedtls_ssl_check_cert_usage(); 
extern int  mbedtls_ssl_check_curve(); 
extern int  mbedtls_ssl_check_sig_hash(); 
extern int  mbedtls_ssl_close_notify(); 
extern int  mbedtls_ssl_conf_alpn_protocols(); 
extern int  mbedtls_ssl_conf_arc4_support(); 
extern int  mbedtls_ssl_conf_authmode(); 
extern int  mbedtls_ssl_conf_ca_chain(); 
extern int  mbedtls_ssl_conf_cbc_record_splitting(); 
extern int  mbedtls_ssl_conf_cert_profile(); 
extern int  mbedtls_ssl_conf_cert_req_ca_list(); 
extern int  mbedtls_ssl_conf_ciphersuites(); 
extern int  mbedtls_ssl_conf_ciphersuites_for_version(); 
extern int  mbedtls_ssl_conf_curves(); 
extern int  mbedtls_ssl_conf_dbg(); 
extern int  mbedtls_ssl_conf_dh_param(); 
extern int  mbedtls_ssl_conf_dh_param_ctx(); 
extern int  mbedtls_ssl_conf_dhm_min_bitlen(); 
extern int  mbedtls_ssl_conf_dtls_anti_replay(); 
extern int  mbedtls_ssl_conf_dtls_badmac_limit(); 
extern int  mbedtls_ssl_conf_encrypt_then_mac(); 
extern int  mbedtls_ssl_conf_endpoint(); 
extern int  mbedtls_ssl_conf_export_keys_cb(); 
extern int  mbedtls_ssl_conf_extended_master_secret(); 
extern int  mbedtls_ssl_conf_fallback(); 
extern int  mbedtls_ssl_conf_handshake_timeout(); 
extern int  mbedtls_ssl_conf_legacy_renegotiation(); 
extern int  mbedtls_ssl_conf_max_frag_len(); 
extern int  mbedtls_ssl_conf_max_version(); 
extern int  mbedtls_ssl_conf_min_version(); 
extern int  mbedtls_ssl_conf_own_cert(); 
extern int  mbedtls_ssl_conf_psk(); 
extern int  mbedtls_ssl_conf_psk_cb(); 
extern int  mbedtls_ssl_conf_read_timeout(); 
extern int  mbedtls_ssl_conf_renegotiation(); 
extern int  mbedtls_ssl_conf_renegotiation_enforced(); 
extern int  mbedtls_ssl_conf_renegotiation_period(); 
extern int  mbedtls_ssl_conf_rng(); 
extern int  mbedtls_ssl_conf_session_cache(); 
extern int  mbedtls_ssl_conf_session_tickets(); 
extern int  mbedtls_ssl_conf_session_tickets_cb(); 
extern int  mbedtls_ssl_conf_sig_hashes(); 
extern int  mbedtls_ssl_conf_sni(); 
extern int  mbedtls_ssl_conf_transport(); 
extern int  mbedtls_ssl_conf_truncated_hmac(); 
extern int  mbedtls_ssl_conf_verify(); 
extern int  mbedtls_ssl_config_defaults(); 
extern int  mbedtls_ssl_config_free(); 
extern int  mbedtls_ssl_config_init(); 
extern int  mbedtls_ssl_derive_keys(); 
extern int  mbedtls_ssl_dtls_replay_check(); 
extern int  mbedtls_ssl_dtls_replay_update(); 
extern int  mbedtls_ssl_fetch_input(); 
extern int  mbedtls_ssl_flush_output(); 
extern int  mbedtls_ssl_free(); 
extern int  mbedtls_ssl_get_alpn_protocol(); 
extern int  mbedtls_ssl_get_bio(); 
extern int  mbedtls_ssl_get_bytes_avail(); 
extern int  mbedtls_ssl_get_ciphersuite(); 
extern int  mbedtls_ssl_get_max_frag_len(); 
extern int  mbedtls_ssl_get_peer_cert(); 
extern int  mbedtls_ssl_get_record_expansion(); 
extern int  mbedtls_ssl_get_session(); 
extern int  mbedtls_ssl_get_verify_result(); 
extern int  mbedtls_ssl_get_version(); 
extern int  mbedtls_ssl_handle_message_type(); 
extern int  mbedtls_ssl_handshake(); 
extern int  mbedtls_ssl_handshake_free(); 
extern int  mbedtls_ssl_handshake_step(); 
extern int  mbedtls_ssl_handshake_wrapup(); 
extern int  mbedtls_ssl_hash_from_md_alg(); 
extern int  mbedtls_ssl_init(); 
extern int  mbedtls_ssl_md_alg_from_hash(); 
extern int  mbedtls_ssl_optimize_checksum(); 
extern int  mbedtls_ssl_parse_certificate(); 
extern int  mbedtls_ssl_parse_change_cipher_spec(); 
extern int  mbedtls_ssl_parse_finished(); 
extern int  mbedtls_ssl_pk_alg_from_sig(); 
extern int  mbedtls_ssl_prepare_handshake_record(); 
extern int  mbedtls_ssl_psk_derive_premaster(); 
extern int  mbedtls_ssl_read(); 
extern int  mbedtls_ssl_read_record(); 
extern int  mbedtls_ssl_read_record_layer(); 
extern int  mbedtls_ssl_read_version(); 
extern int  mbedtls_ssl_recv_flight_completed(); 
extern int  mbedtls_ssl_renegotiate(); 
extern int  mbedtls_ssl_resend(); 
extern int  mbedtls_ssl_reset_checksum(); 
extern int  mbedtls_ssl_send_alert_message(); 
extern int  mbedtls_ssl_send_fatal_handshake_failure(); 
extern int  mbedtls_ssl_send_flight_completed(); 
extern int  mbedtls_ssl_session_free(); 
extern int  mbedtls_ssl_session_init(); 
extern int  mbedtls_ssl_session_reset(); 
extern int  mbedtls_ssl_set_bio(); 
extern int  mbedtls_ssl_set_calc_verify_md(); 
extern int  mbedtls_ssl_set_hostname(); 
extern int  mbedtls_ssl_set_hs_authmode(); 
extern int  mbedtls_ssl_set_hs_ca_chain(); 
extern int  mbedtls_ssl_set_hs_own_cert(); 
extern int  mbedtls_ssl_set_hs_psk(); 
extern int  mbedtls_ssl_set_session(); 
extern int  mbedtls_ssl_set_timer_cb(); 
extern int  mbedtls_ssl_setup(); 
extern int  mbedtls_ssl_sig_from_pk(); 
extern int  mbedtls_ssl_sig_from_pk_alg(); 
extern int  mbedtls_ssl_sig_hash_set_add(); 
extern int  mbedtls_ssl_sig_hash_set_const_hash(); 
extern int  mbedtls_ssl_sig_hash_set_find(); 
extern int  mbedtls_ssl_transform_free(); 
extern int  mbedtls_ssl_update_handshake_status(); 
extern int  mbedtls_ssl_write(); 
extern int  mbedtls_ssl_write_certificate(); 
extern int  mbedtls_ssl_write_change_cipher_spec(); 
extern int  mbedtls_ssl_write_finished(); 
extern int  mbedtls_ssl_write_record(); 
extern int  mbedtls_ssl_write_version(); 
extern int  mbedtls_set_alarm(); 
extern int  mbedtls_timing_get_delay(); 
extern int  mbedtls_timing_get_timer(); 
extern int  mbedtls_timing_hardclock(); 
extern int  mbedtls_timing_self_test(); 
extern int  mbedtls_timing_set_delay(); 
extern int  mbedtls_version_get_number(); 
extern int  mbedtls_version_get_string(); 
extern int  mbedtls_version_get_string_full(); 
extern int  mbedtls_version_check_feature(); 
extern int  mbedtls_x509_dn_gets(); 
extern int  mbedtls_x509_get_alg(); 
extern int  mbedtls_x509_get_alg_null(); 
extern int  mbedtls_x509_get_ext(); 
extern int  mbedtls_x509_get_name(); 
extern int  mbedtls_x509_get_rsassa_pss_params(); 
extern int  mbedtls_x509_get_serial(); 
extern int  mbedtls_x509_get_sig(); 
extern int  mbedtls_x509_get_sig_alg(); 
extern int  mbedtls_x509_get_time(); 
extern int  mbedtls_x509_key_size_helper(); 
extern int  mbedtls_x509_self_test(); 
extern int  mbedtls_x509_serial_gets(); 
extern int  mbedtls_x509_sig_alg_gets(); 
extern int  mbedtls_x509_time_is_future(); 
extern int  mbedtls_x509_time_is_past(); 
extern int  mbedtls_x509write_crt_der(); 
extern int  mbedtls_x509write_crt_free(); 
extern int  mbedtls_x509write_crt_init(); 
extern int  mbedtls_x509write_crt_pem(); 
extern int  mbedtls_x509write_crt_set_authority_key_identifier(); 
extern int  mbedtls_x509write_crt_set_basic_constraints(); 
extern int  mbedtls_x509write_crt_set_extension(); 
extern int  mbedtls_x509write_crt_set_issuer_key(); 
extern int  mbedtls_x509write_crt_set_issuer_name(); 
extern int  mbedtls_x509write_crt_set_key_usage(); 
extern int  mbedtls_x509write_crt_set_md_alg(); 
extern int  mbedtls_x509write_crt_set_ns_cert_type(); 
extern int  mbedtls_x509write_crt_set_serial(); 
extern int  mbedtls_x509write_crt_set_subject_key(); 
extern int  mbedtls_x509write_crt_set_subject_key_identifier(); 
extern int  mbedtls_x509write_crt_set_subject_name(); 
extern int  mbedtls_x509write_crt_set_validity(); 
extern int  mbedtls_x509write_crt_set_version(); 
extern int  mbedtls_x509write_csr_der(); 
extern int  mbedtls_x509write_csr_free(); 
extern int  mbedtls_x509write_csr_init(); 
extern int  mbedtls_x509write_csr_pem(); 
extern int  mbedtls_x509write_csr_set_extension(); 
extern int  mbedtls_x509write_csr_set_key(); 
extern int  mbedtls_x509write_csr_set_key_usage(); 
extern int  mbedtls_x509write_csr_set_md_alg(); 
extern int  mbedtls_x509write_csr_set_ns_cert_type(); 
extern int  mbedtls_x509write_csr_set_subject_name(); 
extern int  mbedtls_x509_set_extension(); 
extern int  mbedtls_x509_string_to_names(); 
extern int  mbedtls_x509_write_extensions(); 
extern int  mbedtls_x509_write_names(); 
extern int  mbedtls_x509_write_sig(); 
extern int  mbedtls_x509_crl_free(); 
extern int  mbedtls_x509_crl_info(); 
extern int  mbedtls_x509_crl_init(); 
extern int  mbedtls_x509_crl_parse(); 
extern int  mbedtls_x509_crl_parse_der(); 
extern int  mbedtls_x509_crl_parse_file(); 
extern int  mbedtls_x509_crt_check_extended_key_usage(); 
extern int  mbedtls_x509_crt_check_key_usage(); 
extern int  mbedtls_x509_crt_free(); 
extern int  mbedtls_x509_crt_info(); 
extern int  mbedtls_x509_crt_init(); 
extern int  mbedtls_x509_crt_is_revoked(); 
extern int  mbedtls_x509_crt_parse(); 
extern int  mbedtls_x509_crt_parse_der(); 
extern int  mbedtls_x509_crt_parse_file(); 
extern int  mbedtls_x509_crt_parse_path(); 
extern int  mbedtls_x509_crt_verify(); 
extern int  mbedtls_x509_crt_verify_info(); 
extern int  mbedtls_x509_crt_verify_with_profile(); 
extern int  mbedtls_x509_csr_free(); 
extern int  mbedtls_x509_csr_info(); 
extern int  mbedtls_x509_csr_init(); 
extern int  mbedtls_x509_csr_parse(); 
extern int  mbedtls_x509_csr_parse_der(); 
extern int  mbedtls_x509_csr_parse_file(); 
extern int  mbedtls_xtea_crypt_cbc(); 
extern int  mbedtls_xtea_crypt_ecb(); 
extern int  mbedtls_xtea_free(); 
extern int  mbedtls_xtea_init(); 
extern int  mbedtls_xtea_self_test(); 
extern int  mbedtls_xtea_setup(); 
extern int  zlibShellInit(); 
extern int  adler32(); 
extern int  adler32_combine(); 
extern int  adler32_combine64(); 
extern int  compress(); 
extern int  compress2(); 
extern int  compressBound(); 
extern int  crc32(); 
extern int  crc32_combine(); 
extern int  crc32_combine64(); 
extern int  get_crc_table(); 
extern int  deflate(); 
extern int  deflateBound(); 
extern int  deflateCopy(); 
extern int  deflateEnd(); 
extern int  deflateInit_(); 
extern int  deflateInit2_(); 
extern int  deflateParams(); 
extern int  deflatePending(); 
extern int  deflatePrime(); 
extern int  deflateReset(); 
extern int  deflateResetKeep(); 
extern int  deflateSetDictionary(); 
extern int  deflateSetHeader(); 
extern int  deflateTune(); 
extern int  test_compress(); 
extern int  test_deflate(); 
extern int  test_dict_deflate(); 
extern int  test_dict_inflate(); 
extern int  test_flush(); 
extern int  test_gzio(); 
extern int  test_inflate(); 
extern int  test_large_deflate(); 
extern int  test_large_inflate(); 
extern int  test_sync(); 
extern int  zlib_main(); 
extern int  gzclose(); 
extern int  gz_error(); 
extern int  gzbuffer(); 
extern int  gzclearerr(); 
extern int  gzdopen(); 
extern int  gzeof(); 
extern int  gzerror(); 
extern int  gzoffset(); 
extern int  gzoffset64(); 
extern int  gzopen(); 
extern int  gzopen64(); 
extern int  gzrewind(); 
extern int  gzseek(); 
extern int  gzseek64(); 
extern int  gztell(); 
extern int  gztell64(); 
extern int  gzclose_r(); 
extern int  gzdirect(); 
extern int  gzgetc(); 
extern int  gzgetc_(); 
extern int  gzgets(); 
extern int  gzread(); 
extern int  gzungetc(); 
extern int  gzclose_w(); 
extern int  gzflush(); 
extern int  gzprintf(); 
extern int  gzputc(); 
extern int  gzputs(); 
extern int  gzsetparams(); 
extern int  gzvprintf(); 
extern int  gzwrite(); 
extern int  inflateBack(); 
extern int  inflateBackEnd(); 
extern int  inflateBackInit_(); 
extern int  inflate_fast(); 
extern int  inflate(); 
extern int  inflateCopy(); 
extern int  inflateEnd(); 
extern int  inflateGetDictionary(); 
extern int  inflateGetHeader(); 
extern int  inflateInit_(); 
extern int  inflateInit2_(); 
extern int  inflateMark(); 
extern int  inflatePrime(); 
extern int  inflateReset(); 
extern int  inflateReset2(); 
extern int  inflateResetKeep(); 
extern int  inflateSetDictionary(); 
extern int  inflateSync(); 
extern int  inflateSyncPoint(); 
extern int  inflateUndermine(); 
extern int  inflate_table(); 
extern int  file_compress(); 
extern int  file_uncompress(); 
extern int  gz_compress(); 
extern int  gz_uncompress(); 
extern int  minigzip_main(); 
extern int  _tr_align(); 
extern int  _tr_flush_bits(); 
extern int  _tr_flush_block(); 
extern int  _tr_init(); 
extern int  _tr_stored_block(); 
extern int  _tr_tally(); 
extern int  uncompress(); 
extern int  zcalloc(); 
extern int  zcfree(); 
extern int  zError(); 
extern int  zlibCompileFlags(); 
extern int  zlibVersion(); 
extern int  backtrace(); 
extern int  armA1xL2CtlGet(); 
extern int  armA1xL2CtlSet(); 
extern int  armAsyncBusMode(); 
extern int  armBranchPredictionDisable(); 
extern int  armBranchPredictionEnable(); 
extern int  armBranchPredictorInvalidate(); 
extern int  armCp15AuxCtrlReg(); 
extern int  armCp15ControlReg(); 
extern int  armCp15LtbTypeReg(); 
extern int  armCp15MainIdReg(); 
extern int  armCp15TcmTypeReg(); 
extern int  armFastBusMode(); 
extern int  armHighVectorDisable(); 
extern int  armHighVectorEnable(); 
extern int  armPrivatePeriphBaseGet(); 
extern int  armSyncBusMode(); 
extern int  armVectorBaseAddrSet(); 
extern int  armWaitForInterrupt(); 
extern int  archAssert(); 
extern int  archCtxStackEnd(); 
extern int  archIntCtxSaveReg(); 
extern int  archTaskCtxCreate(); 
extern int  archTaskCtxPrint(); 
extern int  archTaskCtxSetFp(); 
extern int  archTaskCtxShow(); 
extern int  archTaskRegsGet(); 
extern int  archTaskRegsSet(); 
extern int  archCrtCtxSwitch(); 
extern int  archIntCtxLoad(); 
extern int  archSigCtxLoad(); 
extern int  archTaskCtxCopy(); 
extern int  archTaskCtxStart(); 
extern int  archTaskCtxSwitch(); 
extern int  archAbtHandle(); 
extern int  archSwiHandle(); 
extern int  archUndHandle(); 
extern int  archAbtEntry(); 
extern int  archIntEntry(); 
extern int  archPreEntry(); 
extern int  archSwiEntry(); 
extern int  archUndEntry(); 
extern int  archFindLsb(); 
extern int  archFindMsb(); 
extern int  archBogoMipsLoop(); 
extern int  archGetCpsr(); 
extern int  archPageCopy(); 
extern int  archReboot(); 
extern int  longjmp(); 
extern int  setjmp(); 
extern int  siglongjmp(); 
extern int  sigsetjmp(); 
extern int  archDbgAbInsert(); 
extern int  archDbgApRemove(); 
extern int  archDbgBpAdjust(); 
extern int  archDbgBpInsert(); 
extern int  archDbgBpPrefetch(); 
extern int  archDbgBpRemove(); 
extern int  archDbgTrapType(); 
extern int  archGdbCoreXml(); 
extern int  archGdbGetNextPc(); 
extern int  archGdbGetStepSkip(); 
extern int  archGdbRegGetPc(); 
extern int  archGdbRegSetPc(); 
extern int  archGdbRegsGet(); 
extern int  archGdbRegsSet(); 
extern int  archGdbTargetXml(); 
extern int  armGetNextPc(); 
extern int  armDmaPl330Add(); 
extern int  armDmaPl330GetFuncs(); 
extern int  armDmaPl330Isr(); 
extern int  archElfGotInit(); 
extern int  archElfRelocateRel(); 
extern int  archElfRelocateRela(); 
extern int  archElfRGetJmpBuffItemLen(); 
extern int  dl_unwind_find_exidx(); 
extern int  armVfp9PrimaryInit(); 
extern int  armVfp9SecondaryInit(); 
extern int  armVfp9Disable(); 
extern int  armVfp9Enable(); 
extern int  armVfp9IsEnable(); 
extern int  armVfp9Restore16(); 
extern int  armVfp9Restore32(); 
extern int  armVfp9Save16(); 
extern int  armVfp9Save32(); 
extern int  armVfp9Sid(); 
extern int  armVfp11PrimaryInit(); 
extern int  armVfp11SecondaryInit(); 
extern int  armVfp11Mvfr0(); 
extern int  armVfp11NonSecEn(); 
extern int  armVfpV3PrimaryInit(); 
extern int  armVfpV3SecondaryInit(); 
extern int  armVfpV3Restore16(); 
extern int  armVfpV3Restore32(); 
extern int  armVfpV3Save16(); 
extern int  armVfpV3Save32(); 
extern int  armVfpV4PrimaryInit(); 
extern int  armVfpV4SecondaryInit(); 
extern int  armVfpNonePrimaryInit(); 
extern int  armVfpNoneSecondaryInit(); 
extern int  archFpuCtxInit(); 
extern int  archFpuCtxShow(); 
extern int  archFpuDisable(); 
extern int  archFpuEnable(); 
extern int  archFpuPrimaryInit(); 
extern int  archFpuRestore(); 
extern int  archFpuSave(); 
extern int  archFpuSecondaryInit(); 
extern int  archFpuUndHandle(); 
extern int  armL2Clear(); 
extern int  armL2ClearAll(); 
extern int  armL2Disable(); 
extern int  armL2Enable(); 
extern int  armL2Flush(); 
extern int  armL2FlushAll(); 
extern int  armL2Init(); 
extern int  armL2Invalidate(); 
extern int  armL2InvalidateAll(); 
extern int  armL2IsEnable(); 
extern int  armL2Name(); 
extern int  armL2Sync(); 
extern int  armL2A17Init(); 
extern int  armL2A8Init(); 
extern int  armL2x0Init(); 
extern int  armCacheV4Init(); 
extern int  armCacheV4Reset(); 
extern int  armDCacheV4ClearAll(); 
extern int  armDCacheV4Disable(); 
extern int  armDCacheV4FlushAll(); 
extern int  armDCacheV4InvalidateAll(); 
extern int  armCacheV5Init(); 
extern int  armCacheV5Reset(); 
extern int  armDCacheV5ClearAll(); 
extern int  armDCacheV5Disable(); 
extern int  armDCacheV5FlushAll(); 
extern int  armDCacheV5InvalidateAll(); 
extern int  armCacheV6Init(); 
extern int  armCacheV6Reset(); 
extern int  armDCacheV6ClearAll(); 
extern int  armDCacheV6Disable(); 
extern int  armDCacheV6FlushAll(); 
extern int  armDCacheV6InvalidateAll(); 
extern int  armCacheV7Init(); 
extern int  armCacheV7Reset(); 
extern int  armCacheV7CCSIDR(); 
extern int  armCacheV7CLIDR(); 
extern int  armCacheV7CSSELR(); 
extern int  armCacheV7CTR(); 
extern int  armCacheV7SetCSSELR(); 
extern int  armDCacheV7ClearAll(); 
extern int  armDCacheV7ClearAllPoU(); 
extern int  armDCacheV7Disable(); 
extern int  armDCacheV7FlushAll(); 
extern int  armDCacheV7FlushAllPoU(); 
extern int  armDCacheV7FlushPoU(); 
extern int  armDCacheV7InvalidateAll(); 
extern int  armDCacheV7InvalidateAllPoU(); 
extern int  armCacheV8Init(); 
extern int  armCacheV8Reset(); 
extern int  armCacheRetireDefault(); 
extern int  armCacheRetireRR(); 
extern int  armCacheTypeReg(); 
extern int  armDCacheClear(); 
extern int  armDCacheEnable(); 
extern int  armDCacheFlush(); 
extern int  armDCacheInvalidate(); 
extern int  armICacheDisable(); 
extern int  armICacheEnable(); 
extern int  armICacheInvalidate(); 
extern int  armICacheInvalidateAll(); 
extern int  armMmuV4Init(); 
extern int  armMmuV7ForceDevType(); 
extern int  armMmuV7ForceNonSecure(); 
extern int  armMmuV7ForceShare(); 
extern int  armMmuV7Init(); 
extern int  armMmuV7GetTTBCR(); 
extern int  armMmuV7SetTTBCR(); 
extern int  armGetAbtAddr(); 
extern int  armGetAbtType(); 
extern int  armGetPreAddr(); 
extern int  armGetPreType(); 
extern int  armMmuAbtFaultAddr(); 
extern int  armMmuAbtFaultStatus(); 
extern int  armMmuDisable(); 
extern int  armMmuDisableAlignFault(); 
extern int  armMmuDisableWriteBuffer(); 
extern int  armMmuEnable(); 
extern int  armMmuEnableAlignFault(); 
extern int  armMmuEnableWriteBuffer(); 
extern int  armMmuInitSysRom(); 
extern int  armMmuInvalidateTLB(); 
extern int  armMmuInvalidateTLBMVA(); 
extern int  armMmuPreFaultStatus(); 
extern int  armMmuSetDomain(); 
extern int  armMmuSetProcessId(); 
extern int  armMmuSetTTBase(); 
extern int  armMmuSetTTBase1(); 
extern int  archCacheInit(); 
extern int  archCacheReset(); 
extern int  archMmuInit(); 
extern int  armScuAccessCtrlSet(); 
extern int  armScuCpuMpStatus(); 
extern int  armScuCpuNumber(); 
extern int  armScuCpuPowerStatusGet(); 
extern int  armScuFeatureDisable(); 
extern int  armScuFeatureEnable(); 
extern int  armScuFeatureGet(); 
extern int  armScuFilteringSet(); 
extern int  armScuNonAccessCtrlSet(); 
extern int  armScuSecureInvalidateAll(); 
extern int  armScuTagRamSize(); 
extern int  archSpinBypass(); 
extern int  archSpinDelay(); 
extern int  archSpinInit(); 
extern int  archSpinLock(); 
extern int  archSpinLockRaw(); 
extern int  archSpinNotify(); 
extern int  archSpinTryLock(); 
extern int  archSpinTryLockRaw(); 
extern int  archSpinUnlock(); 
extern int  archSpinUnlockRaw(); 
extern int  archSpinWork(); 
extern int  archKernelParam(); 
extern int  archKernelParamGet(); 
extern int  API_DtraceAbortTrap(); 
extern int  API_DtraceBreakpointInsert(); 
extern int  API_DtraceBreakpointRemove(); 
extern int  API_DtraceBreakTrap(); 
extern int  API_DtraceChildSig(); 
extern int  API_DtraceContinueProcess(); 
extern int  API_DtraceContinueThread(); 
extern int  API_DtraceCreate(); 
extern int  API_DtraceDelBreakInfo(); 
extern int  API_DtraceDelete(); 
extern int  API_DtraceGetBreakInfo(); 
extern int  API_DtraceGetFpuRegs(); 
extern int  API_DtraceGetMems(); 
extern int  API_DtraceGetRegs(); 
extern int  API_DtraceIsValid(); 
extern int  API_DtraceProcessThread(); 
extern int  API_DtraceSchedHook(); 
extern int  API_DtraceSetFpuRegs(); 
extern int  API_DtraceSetMems(); 
extern int  API_DtraceSetPid(); 
extern int  API_DtraceSetRegs(); 
extern int  API_DtraceStopProcess(); 
extern int  API_DtraceStopThread(); 
extern int  API_DtraceThreadExtraInfo(); 
extern int  API_DtraceThreadStepGet(); 
extern int  API_DtraceThreadStepSet(); 
extern int  API_DtraceWatchpointInsert(); 
extern int  API_DtraceWatchpointRemove(); 
extern int  API_GdbInit(); 
extern int  API_GdbModuleInit(); 
extern int  API_OpenOCDStep1(); 
extern int  API_OpenOCDStep2(); 
extern int  API_OpenOCDStep3(); 
extern int  sja1000Init(); 
extern int  sja1000Isr(); 
extern int  i8237aGetFuncs(); 
extern int  i8259aInit(); 
extern int  i8259aIrq(); 
extern int  i8259aIrqDisable(); 
extern int  i8259aIrqEnable(); 
extern int  i8259aIrqEoi(); 
extern int  i8259aIrqIsEnable(); 
extern int  i8259aIrqIsPending(); 
extern int  dm9000Init(); 
extern int  dm9000Isr(); 
extern int  smethndInit(); 
extern int  smethndNotify(); 
extern int  pciSioExarChan(); 
extern int  pciSioExarInit(); 
extern int  pciSioNetmosInit(); 
extern int  pciStorageAtaInit(); 
extern int  pciStorageNvmeInit(); 
extern int  pciStorageSataInit(); 
extern int  sio16c550Init(); 
extern int  sio16c550Isr(); 
extern int  sio16c550RxIsr(); 
extern int  sio16c550TxIsr(); 
extern int  i8254BuzzerOff(); 
extern int  i8254BuzzerOn(); 
extern int  i8254GetCnt(); 
extern int  i8254Init(); 
extern int  i8254InitAsHtimer(); 
extern int  i8254InitAsTick(); 
extern int  API_DiskCacheCreate(); 
extern int  API_DiskCacheCreateEx(); 
extern int  API_DiskCacheCreateEx2(); 
extern int  API_DiskCacheDelete(); 
extern int  API_DiskCacheSync(); 
extern int  __diskCacheFlushInvCnt(); 
extern int  __diskCacheFlushInvMeta(); 
extern int  __diskCacheFlushInvRange(); 
extern int  __diskCacheFlushList(); 
extern int  __diskCacheFsCallback(); 
extern int  __diskCacheHashAdd(); 
extern int  __diskCacheHashFind(); 
extern int  __diskCacheHashRemove(); 
extern int  __diskCacheIoctl(); 
extern int  __diskCacheMemInit(); 
extern int  __diskCacheMemReset(); 
extern int  __diskCacheNodeAlloc(); 
extern int  __diskCacheNodeFind(); 
extern int  __diskCacheNodeGet(); 
extern int  __diskCacheNodeRead(); 
extern int  __diskCacheRead(); 
extern int  __diskCacheReset(); 
extern int  __diskCacheSortListAdd(); 
extern int  __diskCacheStatusChk(); 
extern int  __diskCacheWrite(); 
extern int  __diskCacheWpCreate(); 
extern int  __diskCacheWpDelete(); 
extern int  __diskCacheWpGetBuffer(); 
extern int  __diskCacheWpPutBuffer(); 
extern int  __diskCacheWpRead(); 
extern int  __diskCacheWpSteal(); 
extern int  __diskCacheWpSync(); 
extern int  __diskCacheWpWrite(); 
extern int  __procFsDiskCacheInit(); 
extern int  __diskCacheListAdd(); 
extern int  __diskCacheListDel(); 
extern int  __diskCacheSync(); 
extern int  API_DiskPartitionFree(); 
extern int  API_DiskPartitionGet(); 
extern int  API_DiskPartitionLinkNumGet(); 
extern int  API_DiskPartitionScan(); 
extern int  API_DiskRiad0Create(); 
extern int  API_DiskRiad0Delete(); 
extern int  API_DiskRiad1Create(); 
extern int  API_DiskRiad1Delete(); 
extern int  API_DiskRiad1Ghost(); 
extern int  __diskRaidBytesPerBlock(); 
extern int  __diskRaidBytesPerSector(); 
extern int  __diskRaidCheck(); 
extern int  __diskRaidTotalSector(); 
extern int  disk_initialize(); 
extern int  disk_ioctl(); 
extern int  disk_read(); 
extern int  disk_status(); 
extern int  disk_write(); 
extern int  __fatFsGetError(); 
extern int  API_FatFsDevCreate(); 
extern int  API_FatFsDevDelete(); 
extern int  API_FatFsDrvInstall(); 
extern int  __filInfoToStat(); 
extern int  __fsAttrToMode(); 
extern int  __fsInfoToStatfs(); 
extern int  __fsModeToAttr(); 
extern int  __fattimeToTime(); 
extern int  __timeToFatTime(); 
extern int  get_fattime(); 
extern int  f_chmod(); 
extern int  f_chmod_ex(); 
extern int  f_close(); 
extern int  f_closedir(); 
extern int  f_expand(); 
extern int  f_getfree(); 
extern int  f_getlabel(); 
extern int  f_getlabel_ex(); 
extern int  f_lseek(); 
extern int  f_mkdir(); 
extern int  f_mkdir_ex(); 
extern int  f_mkfs(); 
extern int  f_mount(); 
extern int  f_mount_ex(); 
extern int  f_open(); 
extern int  f_open_ex(); 
extern int  f_opendir(); 
extern int  f_opendir_ex(); 
extern int  f_read(); 
extern int  f_readdir(); 
extern int  f_rename(); 
extern int  f_rename_ex(); 
extern int  f_setlabel(); 
extern int  f_setlabel_ex(); 
extern int  f_stat(); 
extern int  f_stat_ex(); 
extern int  f_sync(); 
extern int  f_syncfs(); 
extern int  f_truncate(); 
extern int  f_unlink(); 
extern int  f_unlink_ex(); 
extern int  f_utime(); 
extern int  f_utime_ex(); 
extern int  f_write(); 
extern int  ff_convert(); 
extern int  ff_wtoupper(); 
extern int  __fsCheckFileName(); 
extern int  __fsCreateFuncGet(); 
extern int  __fsDiskLinkCounterAdd(); 
extern int  __fsDiskLinkCounterDec(); 
extern int  __fsDiskLinkCounterGet(); 
extern int  __fsPartitionProb(); 
extern int  __fsRegister(); 
extern int  _cdio_list_append(); 
extern int  _cdio_list_begin(); 
extern int  _cdio_list_end(); 
extern int  _cdio_list_find(); 
extern int  _cdio_list_foreach(); 
extern int  _cdio_list_free(); 
extern int  _cdio_list_length(); 
extern int  _cdio_list_new(); 
extern int  _cdio_list_node_data(); 
extern int  _cdio_list_node_free(); 
extern int  _cdio_list_node_next(); 
extern int  _cdio_list_prepend(); 
extern int  iso9660_close(); 
extern int  iso9660_dir_to_name(); 
extern int  iso9660_dirlist_free(); 
extern int  iso9660_dirlist_new(); 
extern int  iso9660_filelist_free(); 
extern int  iso9660_filelist_new(); 
extern int  iso9660_have_rr(); 
extern int  iso9660_ifs_find_lsn(); 
extern int  iso9660_ifs_find_lsn_with_path(); 
extern int  iso9660_ifs_fuzzy_read_superblock(); 
extern int  iso9660_ifs_get_application_id(); 
extern int  iso9660_ifs_get_joliet_level(); 
extern int  iso9660_ifs_get_preparer_id(); 
extern int  iso9660_ifs_get_publisher_id(); 
extern int  iso9660_ifs_get_system_id(); 
extern int  iso9660_ifs_get_volume_id(); 
extern int  iso9660_ifs_get_volumeset_id(); 
extern int  iso9660_ifs_is_xa(); 
extern int  iso9660_ifs_read_pvd(); 
extern int  iso9660_ifs_read_superblock(); 
extern int  iso9660_ifs_readdir(); 
extern int  iso9660_ifs_stat(); 
extern int  iso9660_ifs_stat_translate(); 
extern int  iso9660_iso_seek_read(); 
extern int  iso9660_open(); 
extern int  iso9660_open_ext(); 
extern int  iso9660_open_fuzzy(); 
extern int  iso9660_open_fuzzy_ext(); 
extern int  iso9660_stat_free(); 
extern int  cdio_calloc(); 
extern int  cdio_free(); 
extern int  cdio_stdio_destroy(); 
extern int  cdio_stdio_new(); 
extern int  cdio_stream_read(); 
extern int  cdio_stream_seek(); 
extern int  get_rock_ridge_filename(); 
extern int  iso9660_get_posix_filemode_from_rock(); 
extern int  iso9660_get_rock_attr_str(); 
extern int  parse_rock_ridge_stat(); 
extern int  cdio_lba_to_lsn(); 
extern int  cdio_lba_to_msf(); 
extern int  cdio_lba_to_msf_str(); 
extern int  cdio_lsn_to_lba(); 
extern int  cdio_lsn_to_msf(); 
extern int  cdio_mmssff_to_lba(); 
extern int  cdio_msf_to_lba(); 
extern int  cdio_msf_to_lsn(); 
extern int  cdio_msf_to_str(); 
extern int  cdio_msf3_to_lba(); 
extern int  API_Iso9660FsDevCreate(); 
extern int  API_Iso9660FsDevDelete(); 
extern int  API_Iso9660FsDrvInstall(); 
extern int  cdio_charset_converter_create(); 
extern int  cdio_charset_converter_destroy(); 
extern int  cdio_charset_from_utf8(); 
extern int  cdio_charset_to_utf8(); 
extern int  _cdio_memdup(); 
extern int  _cdio_strdup_fixpath(); 
extern int  _cdio_strdup_upper(); 
extern int  _cdio_strfreev(); 
extern int  _cdio_strlenv(); 
extern int  _cdio_strsplit(); 
extern int  cdio_from_bcd8(); 
extern int  cdio_to_bcd8(); 
extern int  iso9660_get_posix_filemode_from_xa(); 
extern int  iso9660_get_xa_attr_str(); 
extern int  iso9660_xa_free(); 
extern int  iso9660_xa_init(); 
extern int  iso9660_dir_add_entry_su(); 
extern int  iso9660_dir_calc_record_size(); 
extern int  iso9660_dir_init_new(); 
extern int  iso9660_dir_init_new_su(); 
extern int  iso9660_dirname_valid_p(); 
extern int  iso9660_get_application_id(); 
extern int  iso9660_get_dir_len(); 
extern int  iso9660_get_dtime(); 
extern int  iso9660_get_ltime(); 
extern int  iso9660_get_posix_filemode(); 
extern int  iso9660_get_preparer_id(); 
extern int  iso9660_get_publisher_id(); 
extern int  iso9660_get_pvd_block_size(); 
extern int  iso9660_get_pvd_id(); 
extern int  iso9660_get_pvd_space_size(); 
extern int  iso9660_get_pvd_type(); 
extern int  iso9660_get_pvd_version(); 
extern int  iso9660_get_root_lsn(); 
extern int  iso9660_get_system_id(); 
extern int  iso9660_get_volume_id(); 
extern int  iso9660_get_volumeset_id(); 
extern int  iso9660_is_achar(); 
extern int  iso9660_is_dchar(); 
extern int  iso9660_name_translate(); 
extern int  iso9660_name_translate_ext(); 
extern int  iso9660_pathname_isofy(); 
extern int  iso9660_pathname_valid_p(); 
extern int  iso9660_pathtable_get_size(); 
extern int  iso9660_pathtable_init(); 
extern int  iso9660_pathtable_l_add_entry(); 
extern int  iso9660_pathtable_m_add_entry(); 
extern int  iso9660_set_dtime(); 
extern int  iso9660_set_dtime_with_timezone(); 
extern int  iso9660_set_evd(); 
extern int  iso9660_set_ltime(); 
extern int  iso9660_set_ltime_with_timezone(); 
extern int  iso9660_set_pvd(); 
extern int  iso9660_strncpy_pad(); 
extern int  API_Mount(); 
extern int  API_MountEx(); 
extern int  API_MountInit(); 
extern int  API_MountShow(); 
extern int  API_Unmount(); 
extern int  mount(); 
extern int  umount(); 
extern int  umount2(); 
extern int  add_mtd_device(); 
extern int  del_mtd_device(); 
extern int  get_mtd_device(); 
extern int  get_mtd_device_nm(); 
extern int  mtd_block_isbad(); 
extern int  mtd_block_markbad(); 
extern int  mtd_erase(); 
extern int  mtd_get_fact_prot_info(); 
extern int  mtd_get_user_prot_info(); 
extern int  mtd_lock(); 
extern int  mtd_lock_user_prot_reg(); 
extern int  mtd_panic_write(); 
extern int  mtd_read(); 
extern int  mtd_read_fact_prot_reg(); 
extern int  mtd_read_oob(); 
extern int  mtd_read_user_prot_reg(); 
extern int  mtd_unlock(); 
extern int  mtd_write(); 
extern int  mtd_write_user_prot_reg(); 
extern int  put_mtd_device(); 
extern int  decode_bch(); 
extern int  encode_bch(); 
extern int  free_bch(); 
extern int  init_bch(); 
extern int  skip_spaces(); 
extern int  strim(); 
extern int  nand_erase_nand(); 
extern int  nand_read_buf(); 
extern int  nand_read_buf16(); 
extern int  nand_read_byte(); 
extern int  nand_release(); 
extern int  nand_scan(); 
extern int  nand_scan_ident(); 
extern int  nand_scan_tail(); 
extern int  nand_wait_ready(); 
extern int  nand_write_buf(); 
extern int  nand_write_buf16(); 
extern int  memchr_inv(); 
extern int  nand_default_bbt(); 
extern int  nand_isbad_bbt(); 
extern int  nand_scan_bbt(); 
extern int  nand_update_bbt(); 
extern int  nand_bch_calculate_ecc(); 
extern int  nand_bch_correct_data(); 
extern int  nand_bch_free(); 
extern int  nand_bch_init(); 
extern int  nand_calculate_ecc(); 
extern int  nand_correct_data(); 
extern int  flexonenand_region(); 
extern int  flexonenand_set_boundary(); 
extern int  onenand_addr(); 
extern int  onenand_bbt_read_oob(); 
extern int  onenand_block(); 
extern int  onenand_block_isbad(); 
extern int  onenand_block_markbad(); 
extern int  onenand_erase(); 
extern int  onenand_print_device_info(); 
extern int  onenand_probe(); 
extern int  onenand_read(); 
extern int  onenand_read_oob(); 
extern int  onenand_release(); 
extern int  onenand_scan(); 
extern int  onenand_sync(); 
extern int  onenand_write(); 
extern int  onenand_write_oob(); 
extern int  onenand_default_bbt(); 
extern int  onenand_scan_bbt(); 
extern int  API_NandRCacheBlockFree(); 
extern int  API_NandRCacheCreate(); 
extern int  API_NandRCacheDelete(); 
extern int  API_NandRCacheNodeAlloc(); 
extern int  API_NandRCacheNodeFree(); 
extern int  API_NandRCacheNodeGet(); 
extern int  mountproc3_dump_3(); 
extern int  mountproc3_export_3(); 
extern int  mountproc3_mnt_3(); 
extern int  mountproc3_null_3(); 
extern int  mountproc3_umnt_3(); 
extern int  mountproc3_umntall_3(); 
extern int  nfsproc3_access_3(); 
extern int  nfsproc3_commit_3(); 
extern int  nfsproc3_create_3(); 
extern int  nfsproc3_fsinfo_3(); 
extern int  nfsproc3_fsstat_3(); 
extern int  nfsproc3_getattr_3(); 
extern int  nfsproc3_link_3(); 
extern int  nfsproc3_lookup_3(); 
extern int  nfsproc3_mkdir_3(); 
extern int  nfsproc3_mknod_3(); 
extern int  nfsproc3_null_3(); 
extern int  nfsproc3_pathconf_3(); 
extern int  nfsproc3_read_3(); 
extern int  nfsproc3_readdir_3(); 
extern int  nfsproc3_readdirplus_3(); 
extern int  nfsproc3_readlink_3(); 
extern int  nfsproc3_remove_3(); 
extern int  nfsproc3_rename_3(); 
extern int  nfsproc3_rmdir_3(); 
extern int  nfsproc3_setattr_3(); 
extern int  nfsproc3_symlink_3(); 
extern int  nfsproc3_write_3(); 
extern int  API_NfsDevCreate(); 
extern int  API_NfsDevDelete(); 
extern int  API_NfsDrvInstall(); 
extern int  xdr_ACCESS3args(); 
extern int  xdr_ACCESS3res(); 
extern int  xdr_ACCESS3resfail(); 
extern int  xdr_ACCESS3resok(); 
extern int  xdr_COMMIT3args(); 
extern int  xdr_COMMIT3res(); 
extern int  xdr_COMMIT3resfail(); 
extern int  xdr_COMMIT3resok(); 
extern int  xdr_cookie3(); 
extern int  xdr_cookieverf3(); 
extern int  xdr_count3(); 
extern int  xdr_CREATE3args(); 
extern int  xdr_CREATE3res(); 
extern int  xdr_CREATE3resfail(); 
extern int  xdr_CREATE3resok(); 
extern int  xdr_createhow3(); 
extern int  xdr_createmode3(); 
extern int  xdr_createverf3(); 
extern int  xdr_devicedata3(); 
extern int  xdr_dirlist3(); 
extern int  xdr_dirlistplus3(); 
extern int  xdr_diropargs3(); 
extern int  xdr_dirpath(); 
extern int  xdr_entry3(); 
extern int  xdr_entryplus3(); 
extern int  xdr_exportnode(); 
extern int  xdr_exports(); 
extern int  xdr_fattr3(); 
extern int  xdr_fhandle3(); 
extern int  xdr_fileid3(); 
extern int  xdr_filename(); 
extern int  xdr_filename3(); 
extern int  xdr_FSINFO3args(); 
extern int  xdr_FSINFO3res(); 
extern int  xdr_FSINFO3resfail(); 
extern int  xdr_FSINFO3resok(); 
extern int  xdr_FSSTAT3args(); 
extern int  xdr_FSSTAT3res(); 
extern int  xdr_FSSTAT3resfail(); 
extern int  xdr_FSSTAT3resok(); 
extern int  xdr_ftype3(); 
extern int  xdr_GETATTR3args(); 
extern int  xdr_GETATTR3res(); 
extern int  xdr_GETATTR3resok(); 
extern int  xdr_gid3(); 
extern int  xdr_groupnode(); 
extern int  xdr_groups(); 
extern int  xdr_int32(); 
extern int  xdr_LINK3args(); 
extern int  xdr_LINK3res(); 
extern int  xdr_LINK3resfail(); 
extern int  xdr_LINK3resok(); 
extern int  xdr_LOOKUP3args(); 
extern int  xdr_LOOKUP3res(); 
extern int  xdr_LOOKUP3resfail(); 
extern int  xdr_LOOKUP3resok(); 
extern int  xdr_MKDIR3args(); 
extern int  xdr_MKDIR3res(); 
extern int  xdr_MKDIR3resfail(); 
extern int  xdr_MKDIR3resok(); 
extern int  xdr_MKNOD3args(); 
extern int  xdr_MKNOD3res(); 
extern int  xdr_MKNOD3resfail(); 
extern int  xdr_MKNOD3resok(); 
extern int  xdr_mknoddata3(); 
extern int  xdr_mode3(); 
extern int  xdr_mountbody(); 
extern int  xdr_mountlist(); 
extern int  xdr_mountres3(); 
extern int  xdr_mountres3_ok(); 
extern int  xdr_mountstat3(); 
extern int  xdr_name(); 
extern int  xdr_nfs_fh3(); 
extern int  xdr_nfspath(); 
extern int  xdr_nfspath3(); 
extern int  xdr_nfsstat3(); 
extern int  xdr_nfstime3(); 
extern int  xdr_offset3(); 
extern int  xdr_PATHCONF3args(); 
extern int  xdr_PATHCONF3res(); 
extern int  xdr_PATHCONF3resfail(); 
extern int  xdr_PATHCONF3resok(); 
extern int  xdr_post_op_attr(); 
extern int  xdr_post_op_fh3(); 
extern int  xdr_pre_op_attr(); 
extern int  xdr_READ3args(); 
extern int  xdr_READ3res(); 
extern int  xdr_READ3resfail(); 
extern int  xdr_READ3resok(); 
extern int  xdr_READDIR3args(); 
extern int  xdr_READDIR3res(); 
extern int  xdr_READDIR3resfail(); 
extern int  xdr_READDIR3resok(); 
extern int  xdr_READDIRPLUS3args(); 
extern int  xdr_READDIRPLUS3res(); 
extern int  xdr_READDIRPLUS3resfail(); 
extern int  xdr_READDIRPLUS3resok(); 
extern int  xdr_READLINK3args(); 
extern int  xdr_READLINK3res(); 
extern int  xdr_READLINK3resfail(); 
extern int  xdr_READLINK3resok(); 
extern int  xdr_REMOVE3args(); 
extern int  xdr_REMOVE3res(); 
extern int  xdr_REMOVE3resfail(); 
extern int  xdr_REMOVE3resok(); 
extern int  xdr_RENAME3args(); 
extern int  xdr_RENAME3res(); 
extern int  xdr_RENAME3resfail(); 
extern int  xdr_RENAME3resok(); 
extern int  xdr_RMDIR3args(); 
extern int  xdr_RMDIR3res(); 
extern int  xdr_RMDIR3resfail(); 
extern int  xdr_RMDIR3resok(); 
extern int  xdr_sattr3(); 
extern int  xdr_sattrguard3(); 
extern int  xdr_set_atime(); 
extern int  xdr_set_gid3(); 
extern int  xdr_set_mode3(); 
extern int  xdr_set_mtime(); 
extern int  xdr_set_size3(); 
extern int  xdr_set_uid3(); 
extern int  xdr_SETATTR3args(); 
extern int  xdr_SETATTR3res(); 
extern int  xdr_SETATTR3resfail(); 
extern int  xdr_SETATTR3resok(); 
extern int  xdr_size3(); 
extern int  xdr_specdata3(); 
extern int  xdr_stable_how(); 
extern int  xdr_SYMLINK3args(); 
extern int  xdr_SYMLINK3res(); 
extern int  xdr_SYMLINK3resfail(); 
extern int  xdr_SYMLINK3resok(); 
extern int  xdr_symlinkdata3(); 
extern int  xdr_time_how(); 
extern int  xdr_uid3(); 
extern int  xdr_uint32(); 
extern int  xdr_uint64(); 
extern int  xdr_wcc_attr(); 
extern int  xdr_wcc_data(); 
extern int  xdr_WRITE3args(); 
extern int  xdr_WRITE3res(); 
extern int  xdr_WRITE3resfail(); 
extern int  xdr_WRITE3resok(); 
extern int  xdr_writeverf3(); 
extern int  __blkIoFsDrvInstall(); 
extern int  API_OemBlkIoCreate(); 
extern int  API_OemBlkIoDelete(); 
extern int  __oemAutoMountDelete(); 
extern int  API_OemDiskGetPath(); 
extern int  API_OemDiskHotplugEventMessage(); 
extern int  API_OemDiskMount(); 
extern int  API_OemDiskMount2(); 
extern int  API_OemDiskMountEx(); 
extern int  API_OemDiskMountEx2(); 
extern int  API_OemDiskMountInit(); 
extern int  API_OemDiskMountShow(); 
extern int  API_OemDiskRemount(); 
extern int  API_OemDiskRemountEx(); 
extern int  API_OemDiskUnmount(); 
extern int  API_OemDiskUnmountEx(); 
extern int  API_OemFdisk(); 
extern int  API_OemFdiskGet(); 
extern int  API_OemFdiskShow(); 
extern int  API_ProcFsDevCreate(); 
extern int  API_ProcFsDrvInstall(); 
extern int  __procFsFindNode(); 
extern int  __procFsRemoveNode(); 
extern int  API_ProcFsAllocNodeBuffer(); 
extern int  API_ProcFsFreeNodeBuffer(); 
extern int  API_ProcFsMakeNode(); 
extern int  API_ProcFsNodeBuffer(); 
extern int  API_ProcFsNodeBufferSize(); 
extern int  API_ProcFsNodeGetRealFileSize(); 
extern int  API_ProcFsNodeMessageValue(); 
extern int  API_ProcFsNodeSetRealFileSize(); 
extern int  API_ProcFsRemoveNode(); 
extern int  __procFsBspInfoInit(); 
extern int  __procFssupInit(); 
extern int  __procFsHookInit(); 
extern int  __procFsKernelInfoInit(); 
extern int  __procFsPowerInit(); 
extern int  API_RamFsDevCreate(); 
extern int  API_RamFsDevDelete(); 
extern int  API_RamFsDrvInstall(); 
extern int  __ram_close(); 
extern int  __ram_increase(); 
extern int  __ram_maken(); 
extern int  __ram_mount(); 
extern int  __ram_move(); 
extern int  __ram_open(); 
extern int  __ram_read(); 
extern int  __ram_stat(); 
extern int  __ram_statfs(); 
extern int  __ram_truncate(); 
extern int  __ram_unlink(); 
extern int  __ram_unmount(); 
extern int  __ram_write(); 
extern int  API_RomFsDevCreate(); 
extern int  API_RomFsDevDelete(); 
extern int  API_RomFsDrvInstall(); 
extern int  __rfs_getfile(); 
extern int  __rfs_lookup(); 
extern int  __rfs_mount(); 
extern int  __rfs_open(); 
extern int  __rfs_path_build_link(); 
extern int  __rfs_pread(); 
extern int  __rfs_readlink(); 
extern int  __rootFsFixName(); 
extern int  __rootFsTimeSet(); 
extern int  API_RootFsDevCreate(); 
extern int  API_RootFsDrvInstall(); 
extern int  API_RootFsTime(); 
extern int  __rootFsDevMatch(); 
extern int  __rootFsFindNode(); 
extern int  __rootFsReadNode(); 
extern int  API_RootFsMakeNode(); 
extern int  API_RootFsRemoveNode(); 
extern int  API_RootFsMap(); 
extern int  API_RootFsMapInit(); 
extern int  tpsFsBtreeAdjustBP(); 
extern int  tpsFsBtreeAllocBlk(); 
extern int  tpsFsBtreeAppendBlk(); 
extern int  tpsFsBtreeBlkCnt(); 
extern int  tpsFsBtreeDump(); 
extern int  tpsFsBtreeFreeBlk(); 
extern int  tpsFsBtreeGetBlk(); 
extern int  tpsFsBtreeGetBlkCnt(); 
extern int  tpsFsBtreeGetLevel(); 
extern int  tpsFsBtreeGetNode(); 
extern int  tpsFsBtreeInit(); 
extern int  tpsFsBtreeInitBP(); 
extern int  tpsFsBtreeInsertNode(); 
extern int  tpsFsBtreeReadBP(); 
extern int  tpsFsBtreeRemoveNode(); 
extern int  tpsFsBtreeTrunc(); 
extern int  tpsFsInodeDump(); 
extern int  tpsFsDevBufRead(); 
extern int  tpsFsDevBufSync(); 
extern int  tpsFsDevBufTrim(); 
extern int  tpsFsDevBufWrite(); 
extern int  tpsFsCreateEntry(); 
extern int  tpsFsEntryFree(); 
extern int  tpsFsEntryRead(); 
extern int  tpsFsEntryRemove(); 
extern int  tpsFsFindEntry(); 
extern int  tpsFsGetDirSize(); 
extern int  tpsFsCloseInode(); 
extern int  tpsFsCreateInode(); 
extern int  tpsFsFlushInodeHead(); 
extern int  tpsFsInodeAddRef(); 
extern int  tpsFsInodeAllocBlk(); 
extern int  tpsFsInodeBuffInvalid(); 
extern int  tpsFsInodeDelRef(); 
extern int  tpsFsInodeGetSize(); 
extern int  tpsFsInodeRead(); 
extern int  tpsFsInodeSync(); 
extern int  tpsFsInodeWrite(); 
extern int  tpsFsOpenInode(); 
extern int  tpsFsTruncInode(); 
extern int  tpsFsFlushSuperBlock(); 
extern int  tpsFsFormat(); 
extern int  tpsFsMount(); 
extern int  tpsFsUnmount(); 
extern int  API_TpsFsDevCreate(); 
extern int  API_TpsFsDevDelete(); 
extern int  API_TpsFsDrvInstall(); 
extern int  __tpsFsLoadTransData(); 
extern int  tpsFsBtreeTransFini(); 
extern int  tpsFsBtreeTransInit(); 
extern int  tpsFsTransAllocAndInit(); 
extern int  tpsFsTransCommitAndFree(); 
extern int  tpsFsTransRead(); 
extern int  tpsFsTransRollBackAndFree(); 
extern int  tpsFsTransTrigerChk(); 
extern int  tpsFsTransWrite(); 
extern int  tspFsCheckTrans(); 
extern int  tspFsCompleteTrans(); 
extern int  tpsFsChmod(); 
extern int  tpsFsChown(); 
extern int  tpsFsChtime(); 
extern int  tpsFsClose(); 
extern int  tpsFsFlushHead(); 
extern int  tpsFsFlushInodes(); 
extern int  tpsFsGetmod(); 
extern int  tpsFsGetSize(); 
extern int  tpsFsLink(); 
extern int  tpsFsMkDir(); 
extern int  tpsFsMove(); 
extern int  tpsFsOpen(); 
extern int  tpsFsRead(); 
extern int  tpsFsReadDir(); 
extern int  tpsFsRemove(); 
extern int  tpsFsStat(); 
extern int  tpsFsStatfs(); 
extern int  tpsFsSync(); 
extern int  tpsFsTrunc(); 
extern int  tpsFsVolSync(); 
extern int  tpsFsWrite(); 
extern int  API_TSyncAdd(); 
extern int  API_FsUniqueAlloc(); 
extern int  API_FsUniqueCreate(); 
extern int  API_FsUniqueDelete(); 
extern int  API_FsUniqueFree(); 
extern int  API_FsUniqueIsVal(); 
extern int  yaffs_alloc_raw_obj(); 
extern int  yaffs_alloc_raw_tnode(); 
extern int  yaffs_deinit_raw_tnodes_and_objs(); 
extern int  yaffs_free_raw_obj(); 
extern int  yaffs_free_raw_tnode(); 
extern int  yaffs_init_raw_tnodes_and_objs(); 
extern int  yaffs_attribs_init(); 
extern int  yaffs_get_attribs(); 
extern int  yaffs_load_attribs(); 
extern int  yaffs_load_attribs_oh(); 
extern int  yaffs_load_current_time(); 
extern int  yaffs_set_attribs(); 
extern int  yaffs_check_chunk_bit(); 
extern int  yaffs_clear_chunk_bit(); 
extern int  yaffs_clear_chunk_bits(); 
extern int  yaffs_count_chunk_bits(); 
extern int  yaffs_set_chunk_bit(); 
extern int  yaffs_still_some_chunks(); 
extern int  yaffs_verify_chunk_bit_id(); 
extern int  yaffs_checkpt_close(); 
extern int  yaffs2_checkpt_invalidate_stream(); 
extern int  yaffs2_checkpt_open(); 
extern int  yaffs2_checkpt_rd(); 
extern int  yaffs2_checkpt_wr(); 
extern int  yaffs2_get_checkpt_sum(); 
extern int  yaffs_ecc_calc(); 
extern int  yaffs_ecc_calc_other(); 
extern int  yaffs_ecc_correct(); 
extern int  yaffs_ecc_correct_other(); 
extern int  yaffs_add_find_tnode_0(); 
extern int  yaffs_add_obj_to_dir(); 
extern int  yaffs_addr_to_chunk(); 
extern int  yaffs_bg_gc(); 
extern int  yaffs_block_became_dirty(); 
extern int  yaffs_check_alloc_available(); 
extern int  yaffs_check_ff(); 
extern int  yaffs_chunk_del(); 
extern int  yaffs_clone_str(); 
extern int  yaffs_count_blocks_by_state(); 
extern int  yaffs_count_free_chunks(); 
extern int  yaffs_create_dir(); 
extern int  yaffs_create_file(); 
extern int  yaffs_create_special(); 
extern int  yaffs_create_symlink(); 
extern int  yaffs_deinitialise(); 
extern int  yaffs_del_obj(); 
extern int  yaffs_do_file_wr(); 
extern int  yaffs_file_rd(); 
extern int  yaffs_find_by_name(); 
extern int  yaffs_find_by_number(); 
extern int  yaffs_find_chunk_in_file(); 
extern int  yaffs_find_or_create_by_number(); 
extern int  yaffs_find_tnode_0(); 
extern int  yaffs_flush_file(); 
extern int  yaffs_flush_whole_cache(); 
extern int  yaffs_get_equivalent_obj(); 
extern int  yaffs_get_group_base(); 
extern int  yaffs_get_n_free_chunks(); 
extern int  yaffs_get_obj_inode(); 
extern int  yaffs_get_obj_length(); 
extern int  yaffs_get_obj_link_count(); 
extern int  yaffs_get_obj_name(); 
extern int  yaffs_get_obj_type(); 
extern int  yaffs_get_symlink_alias(); 
extern int  yaffs_get_temp_buffer(); 
extern int  yaffs_get_tnode(); 
extern int  yaffs_get_xattrib(); 
extern int  yaffs_guts_format_dev(); 
extern int  yaffs_guts_initialise(); 
extern int  yaffs_guts_ll_init(); 
extern int  yaffs_handle_chunk_error(); 
extern int  yaffs_handle_defered_free(); 
extern int  yaffs_handle_shadowed_obj(); 
extern int  yaffs_is_non_empty_dir(); 
extern int  yaffs_link_fixup(); 
extern int  yaffs_link_obj(); 
extern int  yaffs_list_xattrib(); 
extern int  yaffs_lost_n_found(); 
extern int  yaffs_max_file_size(); 
extern int  yaffs_oh_size_load(); 
extern int  yaffs_oh_to_size(); 
extern int  yaffs_put_chunk_in_file(); 
extern int  yaffs_release_temp_buffer(); 
extern int  yaffs_remove_xattrib(); 
extern int  yaffs_rename_obj(); 
extern int  yaffs_resize_file(); 
extern int  yaffs_resize_file_down(); 
extern int  yaffs_retype_obj(); 
extern int  yaffs_root(); 
extern int  yaffs_set_obj_name(); 
extern int  yaffs_set_obj_name_from_oh(); 
extern int  yaffs_set_xattrib(); 
extern int  yaffs_skip_rest_of_block(); 
extern int  yaffs_unlink_obj(); 
extern int  yaffs_unlinker(); 
extern int  yaffs_update_dirty_dirs(); 
extern int  yaffs_update_oh(); 
extern int  yaffs_wr_file(); 
extern int  nandmtd_erase_block(); 
extern int  yaffs_get_mtd_device(); 
extern int  yaffs_mtd_drv_install(); 
extern int  yaffs_put_mtd_device(); 
extern int  yaffs_verify_mtd(); 
extern int  nval_del(); 
extern int  nval_get(); 
extern int  nval_hasvalues(); 
extern int  nval_list(); 
extern int  nval_set(); 
extern int  yaffs_deinit_nand(); 
extern int  yaffs_erase_block(); 
extern int  yaffs_init_nand(); 
extern int  yaffs_mark_bad(); 
extern int  yaffs_query_init_block_state(); 
extern int  yaffs_rd_chunk_tags_nand(); 
extern int  yaffs_wr_chunk_tags_nand(); 
extern int  ynorDeinitialise(); 
extern int  ynorEraseBlock(); 
extern int  ynorInitialise(); 
extern int  ynorReadChunk(); 
extern int  ynorWriteChunk(); 
extern int  yaffs_pack_tags1(); 
extern int  yaffs_unpack_tags1(); 
extern int  yaffs_pack_tags2(); 
extern int  yaffs_pack_tags2_tags_only(); 
extern int  yaffs_unpack_tags2(); 
extern int  yaffs_unpack_tags2_tags_only(); 
extern int  yaffs_summary_add(); 
extern int  yaffs_summary_deinit(); 
extern int  yaffs_summary_fetch(); 
extern int  yaffs_summary_gc(); 
extern int  yaffs_summary_init(); 
extern int  yaffs_summary_read(); 
extern int  __yaffsOsInit(); 
extern int  API_YaffsDevCreate(); 
extern int  API_YaffsDevDelete(); 
extern int  API_YaffsDevMountShow(); 
extern int  API_YaffsDevSync(); 
extern int  API_YaffsDrvInstall(); 
extern int  __procFsYaffsInit(); 
extern int  yaffs_calc_tags_ecc(); 
extern int  yaffs_check_tags_ecc(); 
extern int  yaffs_tags_compat_install(); 
extern int  yaffs_tags_marshall_install(); 
extern int  yaffs_skip_verification(); 
extern int  yaffs_verify_blk(); 
extern int  yaffs_verify_blocks(); 
extern int  yaffs_verify_collected_blk(); 
extern int  yaffs_verify_dir(); 
extern int  yaffs_verify_file(); 
extern int  yaffs_verify_file_sane(); 
extern int  yaffs_verify_free_chunks(); 
extern int  yaffs_verify_link(); 
extern int  yaffs_verify_obj(); 
extern int  yaffs_verify_obj_in_dir(); 
extern int  yaffs_verify_objects(); 
extern int  yaffs_verify_oh(); 
extern int  yaffs_verify_special(); 
extern int  yaffs_verify_symlink(); 
extern int  yaffs1_scan(); 
extern int  yaffs_block_ok_for_gc(); 
extern int  yaffs_calc_checkpt_blocks_required(); 
extern int  yaffs_calc_oldest_dirty_seq(); 
extern int  yaffs_checkpoint_save(); 
extern int  yaffs2_checkpt_invalidate(); 
extern int  yaffs2_checkpt_required(); 
extern int  yaffs2_checkpt_restore(); 
extern int  yaffs2_clear_oldest_dirty_seq(); 
extern int  yaffs2_find_oldest_dirty_seq(); 
extern int  yaffs2_find_refresh_block(); 
extern int  yaffs2_handle_hole(); 
extern int  yaffs2_scan_backwards(); 
extern int  yaffs2_update_oldest_dirty_seq(); 
extern int  yaffs_getdevname(); 
extern int  yaffs_start_up(); 
extern int  yaffsfs_CheckMemRegion(); 
extern int  yaffsfs_CurrentTime(); 
extern int  yaffsfs_FindSymlink(); 
extern int  yaffsfs_free(); 
extern int  yaffsfs_GetLastError(); 
extern int  yaffsfs_Lock(); 
extern int  yaffsfs_malloc(); 
extern int  yaffsfs_OSInitialisation(); 
extern int  yaffsfs_PathBuildLink(); 
extern int  yaffsfs_SetError(); 
extern int  yaffsfs_Unlock(); 
extern int  yaffs_access(); 
extern int  yaffs_access_reldev(); 
extern int  yaffs_access_reldir(); 
extern int  yaffs_add_device(); 
extern int  yaffs_chmod(); 
extern int  yaffs_chmod_reldev(); 
extern int  yaffs_chmod_reldir(); 
extern int  yaffs_close(); 
extern int  yaffs_closedir(); 
extern int  yaffs_dev_rewind(); 
extern int  yaffs_do_background_gc(); 
extern int  yaffs_do_background_gc_reldev(); 
extern int  yaffs_dump_dev(); 
extern int  yaffs_dump_dev_reldir(); 
extern int  yaffs_dup(); 
extern int  yaffs_fchmod(); 
extern int  yaffs_fdatasync(); 
extern int  yaffs_fgetxattr(); 
extern int  yaffs_flistxattr(); 
extern int  yaffs_flush(); 
extern int  yaffs_format(); 
extern int  yaffs_format_common(); 
extern int  yaffs_format_reldev(); 
extern int  yaffs_freespace(); 
extern int  yaffs_freespace_common(); 
extern int  yaffs_freespace_reldev(); 
extern int  yaffs_fremovexattr(); 
extern int  yaffs_fsetxattr(); 
extern int  yaffs_fstat(); 
extern int  yaffs_fsync(); 
extern int  yaffs_ftruncate(); 
extern int  yaffs_futime(); 
extern int  yaffs_get_error(); 
extern int  yaffs_get_trace(); 
extern int  yaffs_getdev(); 
extern int  yaffs_getxattr(); 
extern int  yaffs_getxattr_reldev(); 
extern int  yaffs_getxattr_reldir(); 
extern int  yaffs_handle_rw_set(); 
extern int  yaffs_inodecount(); 
extern int  yaffs_inodecount_common(); 
extern int  yaffs_inodecount_reldev(); 
extern int  yaffs_lgetxattr(); 
extern int  yaffs_lgetxattr_reldev(); 
extern int  yaffs_lgetxattr_reldir(); 
extern int  yaffs_link(); 
extern int  yaffs_link_reldir(); 
extern int  yaffs_listxattr(); 
extern int  yaffs_listxattr_reldev(); 
extern int  yaffs_listxattr_reldir(); 
extern int  yaffs_llistxattr(); 
extern int  yaffs_llistxattr_reldev(); 
extern int  yaffs_llistxattr_reldir(); 
extern int  yaffs_lremovexattr(); 
extern int  yaffs_lremovexattr_reldev(); 
extern int  yaffs_lremovexattr_reldir(); 
extern int  yaffs_lseek(); 
extern int  yaffs_lsetxattr(); 
extern int  yaffs_lsetxattr_reldev(); 
extern int  yaffs_lsetxattr_reldir(); 
extern int  yaffs_lstat(); 
extern int  yaffs_lstat_reldev(); 
extern int  yaffs_lstat_reldir(); 
extern int  yaffs_mkdir(); 
extern int  yaffs_mkdir_reldev(); 
extern int  yaffs_mkdir_reldir(); 
extern int  yaffs_mknod(); 
extern int  yaffs_mknod_reldev(); 
extern int  yaffs_mknod_reldir(); 
extern int  yaffs_mount(); 
extern int  yaffs_mount_common(); 
extern int  yaffs_mount_reldev(); 
extern int  yaffs_mount2(); 
extern int  yaffs_mount2_reldev(); 
extern int  yaffs_mount3(); 
extern int  yaffs_mount3_reldev(); 
extern int  yaffs_n_handles(); 
extern int  yaffs_n_handles_reldir(); 
extern int  yaffs_next_dev(); 
extern int  yaffs_open(); 
extern int  yaffs_open_reldev(); 
extern int  yaffs_open_reldir(); 
extern int  yaffs_open_sharing(); 
extern int  yaffs_open_sharing_reldev(); 
extern int  yaffs_open_sharing_reldir(); 
extern int  yaffs_opendir(); 
extern int  yaffs_opendir_reldev(); 
extern int  yaffs_opendir_reldir(); 
extern int  yaffs_pread(); 
extern int  yaffs_pwrite(); 
extern int  yaffs_read(); 
extern int  yaffs_readdir(); 
extern int  yaffs_readdir_fd(); 
extern int  yaffs_readlink(); 
extern int  yaffs_readlink_reldir(); 
extern int  yaffs_remount(); 
extern int  yaffs_remount_common(); 
extern int  yaffs_remount_reldev(); 
extern int  yaffs_remove_device(); 
extern int  yaffs_removexattr(); 
extern int  yaffs_removexattr_reldev(); 
extern int  yaffs_removexattr_reldir(); 
extern int  yaffs_rename(); 
extern int  yaffs_rename_reldev(); 
extern int  yaffs_rename_reldir(); 
extern int  yaffs_rewinddir(); 
extern int  yaffs_rewinddir_fd(); 
extern int  yaffs_rmdir(); 
extern int  yaffs_rmdir_reldev(); 
extern int  yaffs_rmdir_reldir(); 
extern int  yaffs_set_error(); 
extern int  yaffs_set_trace(); 
extern int  yaffs_setxattr(); 
extern int  yaffs_setxattr_reldev(); 
extern int  yaffs_setxattr_reldir(); 
extern int  yaffs_stat(); 
extern int  yaffs_stat_reldev(); 
extern int  yaffs_stat_reldir(); 
extern int  yaffs_symlink(); 
extern int  yaffs_symlink_reldir(); 
extern int  yaffs_sync(); 
extern int  yaffs_sync_common(); 
extern int  yaffs_sync_reldev(); 
extern int  yaffs_totalspace(); 
extern int  yaffs_totalspace_common(); 
extern int  yaffs_totalspace_reldev(); 
extern int  yaffs_truncate(); 
extern int  yaffs_truncate_reldev(); 
extern int  yaffs_truncate_reldir(); 
extern int  yaffs_unlink(); 
extern int  yaffs_unlink_reldev(); 
extern int  yaffs_unlink_reldir(); 
extern int  yaffs_unmount(); 
extern int  yaffs_unmount_reldev(); 
extern int  yaffs_unmount2(); 
extern int  yaffs_unmount2_common(); 
extern int  yaffs_unmount2_reldev(); 
extern int  yaffs_utime(); 
extern int  yaffs_utime_reldev(); 
extern int  yaffs_utime_reldir(); 
extern int  yaffs_write(); 
extern int  yaffsfs_readdir_no_lock(); 
extern int  yaffs_hweight32(); 
extern int  yaffs_hweight8(); 
extern int  yaffs_qsort(); 
extern int  API_GuiInputDevKeyboardHookSet(); 
extern int  API_GuiInputDevMouseHookSet(); 
extern int  API_GuiInputDevProcStart(); 
extern int  API_GuiInputDevProcStop(); 
extern int  API_GuiInputDevReg(); 
extern int  API_CacheAliasProb(); 
extern int  API_CacheBarrier(); 
extern int  API_CacheClear(); 
extern int  API_CacheClearPage(); 
extern int  API_CacheDataUpdate(); 
extern int  API_CacheDisable(); 
extern int  API_CacheDmaClear(); 
extern int  API_CacheDmaFlush(); 
extern int  API_CacheDmaFree(); 
extern int  API_CacheDmaInvalidate(); 
extern int  API_CacheDmaMalloc(); 
extern int  API_CacheDmaMallocAlign(); 
extern int  API_CacheEnable(); 
extern int  API_CacheFlush(); 
extern int  API_CacheFlushPage(); 
extern int  API_CacheFuncsSet(); 
extern int  API_CacheGetLibBlock(); 
extern int  API_CacheGetMode(); 
extern int  API_CacheGetOption(); 
extern int  API_CacheInvalidate(); 
extern int  API_CacheInvalidatePage(); 
extern int  API_CacheLibPrimaryInit(); 
extern int  API_CacheLibSecondaryInit(); 
extern int  API_CacheLine(); 
extern int  API_CacheLocalTextUpdate(); 
extern int  API_CacheLocation(); 
extern int  API_CacheLock(); 
extern int  API_CacheTextUpdate(); 
extern int  API_CacheUnlock(); 
extern int  API_CacheWaySize(); 
extern int  API_CrashDumpBuffer(); 
extern int  API_CrashDumpSave(); 
extern int  API_CrashDumpShow(); 
extern int  _CrashDumpAbortAccess(); 
extern int  _CrashDumpAbortFatal(); 
extern int  _CrashDumpAbortKernel(); 
extern int  _CrashDumpAbortStkOf(); 
extern int  _CrashDumpGet(); 
extern int  _CrashDumpSet(); 
extern int  _BitmapAdd(); 
extern int  _BitmapDel(); 
extern int  _BitmapHigh(); 
extern int  _BitmapInit(); 
extern int  _BitmapIsEmpty(); 
extern int  _CandTableTryAdd(); 
extern int  _CandTableTryDel(); 
extern int  _CandTableUpdate(); 
extern int  _CoroutineFreeAll(); 
extern int  _CoroutineReclaim(); 
extern int  _CoroutineShell(); 
extern int  _CpuActive(); 
extern int  _CpuGetMaxNesting(); 
extern int  _CpuGetNesting(); 
extern int  _CpuGetSchedAffinity(); 
extern int  _CpuSetSchedAffinity(); 
extern int  __errno(); 
extern int  _DebugFmtMsg(); 
extern int  _DebugMessage(); 
extern int  _ErrorHandle(); 
extern int  _EventPrioTryBoost(); 
extern int  _EventPrioTryResume(); 
extern int  _EventReadyFifoLowLevel(); 
extern int  _EventReadyHighLevel(); 
extern int  _EventReadyPriorityLowLevel(); 
extern int  _EventUnQueue(); 
extern int  _EventWaitFifo(); 
extern int  _EventWaitPriority(); 
extern int  _EventInit(); 
extern int  _AddTCBToEventFifo(); 
extern int  _AddTCBToEventPriority(); 
extern int  _DelTCBFromEventFifo(); 
extern int  _DelTCBFromEventPriority(); 
extern int  _EventSetBlock(); 
extern int  _EventSetInit(); 
extern int  _EventSetDeleteReady(); 
extern int  _EventSetThreadReady(); 
extern int  _EventSetUnQueue(); 
extern int  _GlobalPrimaryInit(); 
extern int  _GlobalSecondaryInit(); 
extern int  _HeapInit(); 
extern int  _HeapKernelInit(); 
extern int  _HeapSystemInit(); 
extern int  _HeapAddMemory(); 
extern int  _HeapAllocate(); 
extern int  _HeapAllocateAlign(); 
extern int  _HeapCreate(); 
extern int  _HeapCtor(); 
extern int  _HeapCtorEx(); 
extern int  _HeapDelete(); 
extern int  _HeapDtor(); 
extern int  _HeapFree(); 
extern int  _HeapGetInfo(); 
extern int  _HeapGetMax(); 
extern int  _HeapRealloc(); 
extern int  _HeapTraceAlloc(); 
extern int  _HeapTraceFree(); 
extern int  _HeapVerify(); 
extern int  _HeapZallocate(); 
extern int  _IdleThread(); 
extern int  _InterVectInit(); 
extern int  _ITimerThread(); 
extern int  _ITimerWakeup(); 
extern int  _jobQueueAdd(); 
extern int  _jobQueueCreate(); 
extern int  _jobQueueDel(); 
extern int  _jobQueueDelete(); 
extern int  _jobQueueExec(); 
extern int  _jobQueueFinit(); 
extern int  _jobQueueFlush(); 
extern int  _jobQueueInit(); 
extern int  _jobQueueLost(); 
extern int  _KernelHighLevelInit(); 
extern int  _KernelPrimaryLowLevelInit(); 
extern int  _KernelSecondaryLowLevelInit(); 
extern int  __kernelEnter(); 
extern int  __kernelEnterFunc(); 
extern int  __kernelEnterIrq(); 
extern int  __kernelExit(); 
extern int  __kernelExitIrq(); 
extern int  __kernelIsEnter(); 
extern int  __kernelIsLockByMe(); 
extern int  __kernelOwner(); 
extern int  __kernelSched(); 
extern int  __kernelSchedInt(); 
extern int  __kernelSpaceEnter(); 
extern int  __kernelSpaceExit(); 
extern int  __kernelSpaceGet(); 
extern int  __kernelSpaceGet2(); 
extern int  __kernelSpaceSet(); 
extern int  __kernelSpaceSet2(); 
extern int  _MsgQueueInit(); 
extern int  _MsgQueueClear(); 
extern int  _MsgQueueGet(); 
extern int  _MsgQueueMsgLen(); 
extern int  _MsgQueuePut(); 
extern int  _MakeObjectId(); 
extern int  _PartitionInit(); 
extern int  _PartitionAllocate(); 
extern int  _PartitionFree(); 
extern int  _PriorityInit(); 
extern int  _AddTCBToReadyRing(); 
extern int  _DelTCBFromReadyRing(); 
extern int  _ReadyTableInit(); 
extern int  _ReadyTableAdd(); 
extern int  _ReadyTableDel(); 
extern int  _RmsInit(); 
extern int  _RmsActive(); 
extern int  _RmsEndExpire(); 
extern int  _RmsGetExecTime(); 
extern int  _RmsInitExpire(); 
extern int  _RtcInit(); 
extern int  _SchedCrSwp(); 
extern int  _SchedSafeStack(); 
extern int  _SchedSetPrio(); 
extern int  _SchedSetRet(); 
extern int  _SchedSwp(); 
extern int  _Schedule(); 
extern int  _ScheduleInit(); 
extern int  _ScheduleInt(); 
extern int  _SchedGetCand(); 
extern int  _SchedIsLock(); 
extern int  _SchedTick(); 
extern int  _SchedYield(); 
extern int  _SmpCallFunc(); 
extern int  _SmpCallFuncAllOther(); 
extern int  _SmpPerfIpi(); 
extern int  _SmpProcIpi(); 
extern int  _SmpSendIpi(); 
extern int  _SmpSendIpiAllOther(); 
extern int  _SmpTryProcIpi(); 
extern int  _SmpUpdateIpi(); 
extern int  _SmpSpinInit(); 
extern int  _SmpSpinLock(); 
extern int  _SmpSpinLockIgnIrq(); 
extern int  _SmpSpinLockIrq(); 
extern int  _SmpSpinLockIrqQuick(); 
extern int  _SmpSpinLockRaw(); 
extern int  _SmpSpinLockTask(); 
extern int  _SmpSpinTryLock(); 
extern int  _SmpSpinTryLockIgnIrq(); 
extern int  _SmpSpinTryLockIrq(); 
extern int  _SmpSpinTryLockRaw(); 
extern int  _SmpSpinTryLockTask(); 
extern int  _SmpSpinUnlock(); 
extern int  _SmpSpinUnlockIgnIrq(); 
extern int  _SmpSpinUnlockIrq(); 
extern int  _SmpSpinUnlockIrqQuick(); 
extern int  _SmpSpinUnlockRaw(); 
extern int  _SmpSpinUnlockTask(); 
extern int  _SmpKernelLockIgnIrq(); 
extern int  _SmpKernelLockQuick(); 
extern int  _SmpKernelUnlockIgnIrq(); 
extern int  _SmpKernelUnlockQuick(); 
extern int  _SmpKernelUnlockSched(); 
extern int  _SmpKernTimeLockIgnIrq(); 
extern int  _SmpKernTimeLockQuick(); 
extern int  _SmpKernTimeUnlockIgnIrq(); 
extern int  _SmpKernTimeUnlockQuick(); 
extern int  _StackCheckGuard(); 
extern int  _StackCheckInit(); 
extern int  _StackAllocate(); 
extern int  _StackFree(); 
extern int  _ThreadGetAffinity(); 
extern int  _ThreadOffAffinity(); 
extern int  _ThreadSetAffinity(); 
extern int  _ThreadFpuSwitch(); 
extern int  _ThreadIdInit(); 
extern int  _TCBBuild(); 
extern int  _TCBBuildExt(); 
extern int  _TCBCleanupPopExt(); 
extern int  _TCBDestroy(); 
extern int  _TCBDestroyExt(); 
extern int  _TCBTryRun(); 
extern int  _ThreadDetach(); 
extern int  _ThreadDisjoin(); 
extern int  _ThreadJoin(); 
extern int  _ThreadWjAdd(); 
extern int  _ThreadWjClear(); 
extern int  _ThreadContinue(); 
extern int  _ThreadDebugUnpendSem(); 
extern int  _ThreadDeleteProcHook(); 
extern int  _ThreadDeleteWaitDeath(); 
extern int  _ThreadLock(); 
extern int  _ThreadMigrateToProc(); 
extern int  _ThreadRestartProcHook(); 
extern int  _ThreadSched(); 
extern int  _ThreadStop(); 
extern int  _ThreadTraversal(); 
extern int  _ThreadUnlock(); 
extern int  _ThreadUserGet(); 
extern int  _ThreadSafeInKern(); 
extern int  _ThreadSafeInternal(); 
extern int  _ThreadSafeResume(); 
extern int  _ThreadSafeSuspend(); 
extern int  _ThreadUnsafeInternal(); 
extern int  _ThreadUnsafeInternalEx(); 
extern int  _ThreadShell(); 
extern int  _ThreadStatusChange(); 
extern int  _ThreadStatusChangeCur(); 
extern int  _ThreadUnwaitStatus(); 
extern int  _ThreadVarInit(); 
extern int  __timespecToTickDiff(); 
extern int  __timespecToTickDiff64(); 
extern int  _TimeCvtInit(); 
extern int  _TimerInit(); 
extern int  _ThreadTick(); 
extern int  _WatchDogTick(); 
extern int  _WakeupAdd(); 
extern int  _WakeupDel(); 
extern int  _WakeupStatus(); 
extern int  API_CoroutineCreate(); 
extern int  API_CoroutineDelete(); 
extern int  API_CoroutineExit(); 
extern int  API_CoroutineResume(); 
extern int  API_CoroutineStackCheck(); 
extern int  API_CoroutineYield(); 
extern int  API_CpuCurId(); 
extern int  API_CpuIsRunning(); 
extern int  API_CpuIsUp(); 
extern int  API_CpuNum(); 
extern int  API_CpuPhyId(); 
extern int  API_CpuUp(); 
extern int  API_CpuUpNum(); 
extern int  API_CpuGetSchedAffinity(); 
extern int  API_CpuSetSchedAffinity(); 
extern int  _CpuBogoMipsClear(); 
extern int  API_CpuBogoMips(); 
extern int  API_CpuPowerGet(); 
extern int  API_CpuPowerSet(); 
extern int  API_EventSetCreate(); 
extern int  API_EventSetDelete(); 
extern int  API_EventSetGet(); 
extern int  API_EventSetGetEx(); 
extern int  API_EventSetGetName(); 
extern int  API_EventSetSet(); 
extern int  API_EventSetStatus(); 
extern int  API_EventSetTryGet(); 
extern int  API_EventSetTryGetEx(); 
extern int  API_GetLastError(); 
extern int  API_GetLastErrorEx(); 
extern int  API_SetLastError(); 
extern int  API_SetLastErrorEx(); 
extern int  API_InterContext(); 
extern int  API_InterGetNesting(); 
extern int  API_InterGetNestingById(); 
extern int  _interDeferInit(); 
extern int  API_InterDeferContext(); 
extern int  API_InterDeferGet(); 
extern int  API_InterDeferJobAdd(); 
extern int  API_InterDeferJobDelete(); 
extern int  API_InterEnter(); 
extern int  API_InterExit(); 
extern int  API_InterLock(); 
extern int  API_InterUnlock(); 
extern int  API_InterVectorGetPriority(); 
extern int  API_InterVectorSetPriority(); 
extern int  API_InterStackBaseGet(); 
extern int  API_InterStackCheck(); 
extern int  API_InterGetTarget(); 
extern int  API_InterSetTarget(); 
extern int  API_InterVectorBaseGet(); 
extern int  API_InterVectorConnect(); 
extern int  API_InterVectorConnectEx(); 
extern int  API_InterVectorDisconnect(); 
extern int  API_InterVectorDisconnectEx(); 
extern int  API_InterVectorServiceCnt(); 
extern int  API_InterVectorDisable(); 
extern int  API_InterVectorDisableEx(); 
extern int  API_InterVectorEnable(); 
extern int  API_InterVectorIsEnable(); 
extern int  API_InterVectorGetFlag(); 
extern int  API_InterVectorSetFlag(); 
extern int  API_InterVectorIpi(); 
extern int  API_InterVectorIpiEx(); 
extern int  API_InterVectorIsr(); 
extern int  API_Atomic64Add(); 
extern int  API_Atomic64And(); 
extern int  API_Atomic64Cas(); 
extern int  API_Atomic64Dec(); 
extern int  API_Atomic64Get(); 
extern int  API_Atomic64Inc(); 
extern int  API_Atomic64Nand(); 
extern int  API_Atomic64Or(); 
extern int  API_Atomic64Set(); 
extern int  API_Atomic64Sub(); 
extern int  API_Atomic64Swp(); 
extern int  API_Atomic64Xor(); 
extern int  API_AtomicAdd(); 
extern int  API_AtomicAnd(); 
extern int  API_AtomicCas(); 
extern int  API_AtomicDec(); 
extern int  API_AtomicGet(); 
extern int  API_AtomicInc(); 
extern int  API_AtomicNand(); 
extern int  API_AtomicOr(); 
extern int  API_AtomicSet(); 
extern int  API_AtomicSub(); 
extern int  API_AtomicSwp(); 
extern int  API_AtomicXor(); 
extern int  API_KernelFpuPrimaryInit(); 
extern int  API_KernelFpuSecondaryInit(); 
extern int  API_KernelGetExc(); 
extern int  API_KernelGetIdle(); 
extern int  API_KernelGetItimer(); 
extern int  API_KernelGetPriorityMax(); 
extern int  API_KernelGetPriorityMin(); 
extern int  API_KernelGetThreadNum(); 
extern int  API_KernelGetThreadNumByPriority(); 
extern int  API_KernelHeapInfo(); 
extern int  API_KernelHeapInfoEx(); 
extern int  API_KernelHookGet(); 
extern int  API_KernelHookSet(); 
extern int  API_KernelHookDelete(); 
extern int  API_KernelSmpCall(); 
extern int  API_KernelSmpCallAll(); 
extern int  API_KernelSmpCallAllOther(); 
extern int  API_KernelIsRunning(); 
extern int  API_KernelIsCpuIdle(); 
extern int  API_KernelIsSystemIdle(); 
extern int  API_KernelNop(); 
extern int  API_ObjectGetClass(); 
extern int  API_ObjectGetIndex(); 
extern int  API_ObjectGetNode(); 
extern int  API_ObjectIsGlobal(); 
extern int  API_ObjectShareAdd(); 
extern int  API_ObjectShareDelete(); 
extern int  API_ObjectShareFind(); 
extern int  API_KernelStartParam(); 
extern int  API_KernelStartParamGet(); 
extern int  API_KernelReboot(); 
extern int  API_KernelRebootEx(); 
extern int  API_KernelResume(); 
extern int  API_SpinDestory(); 
extern int  API_SpinInit(); 
extern int  API_SpinLock(); 
extern int  API_SpinLockIrq(); 
extern int  API_SpinLockQuick(); 
extern int  API_SpinRestrict(); 
extern int  API_SpinTryLock(); 
extern int  API_SpinTryLockIrq(); 
extern int  API_SpinUnlock(); 
extern int  API_SpinUnlockIrq(); 
extern int  API_SpinUnlockQuick(); 
extern int  API_KernelPrimaryStart(); 
extern int  API_KernelSecondaryStart(); 
extern int  API_KernelSuspend(); 
extern int  API_KernelTicks(); 
extern int  API_KernelTicksAdjust(); 
extern int  API_KernelTicksContext(); 
extern int  uuidgen(); 
extern int  API_KernelVerinfo(); 
extern int  API_KernelVerpatch(); 
extern int  API_KernelVersion(); 
extern int  API_MsgQueueClear(); 
extern int  API_MsgQueueCreate(); 
extern int  API_MsgQueueDelete(); 
extern int  API_MsgQueueFlush(); 
extern int  API_MsgQueueFlushReceive(); 
extern int  API_MsgQueueFlushSend(); 
extern int  API_MsgQueueGetName(); 
extern int  API_MsgQueueReceive(); 
extern int  API_MsgQueueReceiveEx(); 
extern int  API_MsgQueueSend(); 
extern int  API_MsgQueueSend2(); 
extern int  API_MsgQueueSendEx(); 
extern int  API_MsgQueueSendEx2(); 
extern int  API_MsgQueueStatus(); 
extern int  API_MsgQueueStatusEx(); 
extern int  API_MsgQueueTryReceive(); 
extern int  API_PartitionCreate(); 
extern int  API_PartitionDelete(); 
extern int  API_PartitionDeleteEx(); 
extern int  API_PartitionGet(); 
extern int  API_PartitionGetName(); 
extern int  API_PartitionPut(); 
extern int  API_PartitionStatus(); 
extern int  API_RegionAddMem(); 
extern int  API_RegionCreate(); 
extern int  API_RegionDelete(); 
extern int  API_RegionDeleteEx(); 
extern int  API_RegionGet(); 
extern int  API_RegionGetAlign(); 
extern int  API_RegionGetName(); 
extern int  API_RegionPut(); 
extern int  API_RegionReget(); 
extern int  API_RegionGetMax(); 
extern int  API_RegionStatus(); 
extern int  API_RegionStatusEx(); 
extern int  API_RmsCancel(); 
extern int  API_RmsCreate(); 
extern int  API_RmsDelete(); 
extern int  API_RmsDeleteEx(); 
extern int  API_RmsExecTimeGet(); 
extern int  API_RmsGetName(); 
extern int  API_RmsPeriod(); 
extern int  API_RmsStatus(); 
extern int  API_SemaphoreBClear(); 
extern int  API_SemaphoreBCreate(); 
extern int  API_SemaphoreBDelete(); 
extern int  API_SemaphoreBFlush(); 
extern int  API_SemaphoreBPend(); 
extern int  API_SemaphoreBPendEx(); 
extern int  API_SemaphoreBPost(); 
extern int  API_SemaphoreBPost2(); 
extern int  API_SemaphoreBPostEx(); 
extern int  API_SemaphoreBPostEx2(); 
extern int  API_SemaphoreBRelease(); 
extern int  API_SemaphoreBStatus(); 
extern int  API_SemaphoreBTryPend(); 
extern int  API_SemaphoreCClear(); 
extern int  API_SemaphoreCCreate(); 
extern int  API_SemaphoreCDelete(); 
extern int  API_SemaphoreCFlush(); 
extern int  API_SemaphoreCPend(); 
extern int  API_SemaphoreCPost(); 
extern int  API_SemaphoreCRelease(); 
extern int  API_SemaphoreCStatus(); 
extern int  API_SemaphoreCStatusEx(); 
extern int  API_SemaphoreCTryPend(); 
extern int  API_SemaphoreDelete(); 
extern int  API_SemaphoreFlush(); 
extern int  API_SemaphoreGetName(); 
extern int  API_SemaphoreMCreate(); 
extern int  API_SemaphoreMDelete(); 
extern int  API_SemaphoreMPend(); 
extern int  API_SemaphoreMPost(); 
extern int  API_SemaphoreMStatus(); 
extern int  API_SemaphoreMStatusEx(); 
extern int  API_SemaphorePend(); 
extern int  API_SemaphorePost(); 
extern int  API_SemaphorePostBPend(); 
extern int  API_SemaphorePostCPend(); 
extern int  API_SemaphoreRWCreate(); 
extern int  API_SemaphoreRWDelete(); 
extern int  API_SemaphoreRWPendR(); 
extern int  API_SemaphoreRWPendW(); 
extern int  API_SemaphoreRWPost(); 
extern int  API_SemaphoreRWStatus(); 
extern int  API_ThreadGetAffinity(); 
extern int  API_ThreadSetAffinity(); 
extern int  API_ThreadAttrBuild(); 
extern int  API_ThreadAttrBuildDefault(); 
extern int  API_ThreadAttrBuildEx(); 
extern int  API_ThreadAttrBuildFP(); 
extern int  API_ThreadAttrGet(); 
extern int  API_ThreadAttrGetDefault(); 
extern int  API_ThreadAttrSetArg(); 
extern int  API_ThreadAttrSetGuardSize(); 
extern int  API_ThreadAttrSetStackSize(); 
extern int  API_ThreadCancel(); 
extern int  API_ThreadCancelWatchDog(); 
extern int  API_ThreadCPUUsageRefresh(); 
extern int  API_ThreadCreate(); 
extern int  __threadDelete(); 
extern int  _exit(); 
extern int  API_ThreadDelete(); 
extern int  API_ThreadExit(); 
extern int  atexit(); 
extern int  exit(); 
extern int  API_ThreadDesc(); 
extern int  API_ThreadDetach(); 
extern int  API_ThreadDetachEx(); 
extern int  API_ThreadFeedWatchDog(); 
extern int  API_ThreadForceDelete(); 
extern int  API_ThreadForceResume(); 
extern int  API_ThreadCPUUsageIsOn(); 
extern int  API_ThreadCPUUsageOff(); 
extern int  API_ThreadCPUUsageOn(); 
extern int  API_ThreadGetCPUUsage(); 
extern int  API_ThreadGetCPUUsageAll(); 
extern int  API_ThreadGetName(); 
extern int  API_ThreadCurNotePad(); 
extern int  API_ThreadGetNotePad(); 
extern int  API_ThreadGetPriority(); 
extern int  API_ThreadGetSchedParam(); 
extern int  API_ThreadGetSlice(); 
extern int  API_ThreadGetSliceEx(); 
extern int  API_ThreadGetStackMini(); 
extern int  API_ThreadIdInter(); 
extern int  API_ThreadIdSelf(); 
extern int  API_ThreadTcbInter(); 
extern int  API_ThreadTcbSelf(); 
extern int  API_ThreadInit(); 
extern int  API_ThreadIsLock(); 
extern int  API_ThreadIsReady(); 
extern int  API_ThreadIsRunning(); 
extern int  API_ThreadIsSafe(); 
extern int  API_ThreadIsSuspend(); 
extern int  API_ThreadJoin(); 
extern int  API_ThreadLock(); 
extern int  API_ThreadRestart(); 
extern int  API_ThreadRestartEx(); 
extern int  API_ThreadResume(); 
extern int  API_ThreadSafe(); 
extern int  API_ThreadSetCancelState(); 
extern int  API_ThreadSetCancelType(); 
extern int  API_ThreadSetName(); 
extern int  API_ThreadSetNotePad(); 
extern int  API_ThreadSetPriority(); 
extern int  API_ThreadSetSchedParam(); 
extern int  API_ThreadSetSlice(); 
extern int  API_ThreadStackCheck(); 
extern int  API_ThreadStart(); 
extern int  API_ThreadStartEx(); 
extern int  API_ThreadContinue(); 
extern int  API_ThreadStop(); 
extern int  API_ThreadSuspend(); 
extern int  API_ThreadTestCancel(); 
extern int  API_ThreadUnlock(); 
extern int  API_ThreadUnsafe(); 
extern int  API_ThreadVerify(); 
extern int  API_ThreadWakeup(); 
extern int  API_ThreadWakeupEx(); 
extern int  API_ThreadYield(); 
extern int  API_TimeGet(); 
extern int  API_TimeGet64(); 
extern int  API_TimeGetFrequency(); 
extern int  API_TimeSet(); 
extern int  API_TimerCancel(); 
extern int  API_TimerCreate(); 
extern int  API_TimerDelete(); 
extern int  API_TimerGetName(); 
extern int  API_TimerHGetFrequency(); 
extern int  API_TimerHTicks(); 
extern int  API_TimerReset(); 
extern int  API_TimerStart(); 
extern int  API_TimerStartEx(); 
extern int  API_TimerStatus(); 
extern int  API_TimerStatusEx(); 
extern int  API_TimeMSleep(); 
extern int  API_TimeNanoSleepMethod(); 
extern int  API_TimeSleep(); 
extern int  API_TimeSleepEx(); 
extern int  API_TimeSleepUntil(); 
extern int  API_TimeSSleep(); 
extern int  nanosleep(); 
extern int  sleep(); 
extern int  usleep(); 
extern int  API_TimeTodAdj(); 
extern int  API_TimeTodAdjEx(); 
extern int  getegid(); 
extern int  geteuid(); 
extern int  getgid(); 
extern int  getgroups(); 
extern int  getlogin(); 
extern int  getlogin_r(); 
extern int  getuid(); 
extern int  setegid(); 
extern int  seteuid(); 
extern int  setgid(); 
extern int  setgroups(); 
extern int  setuid(); 
extern int  API_WorkQueueCreate(); 
extern int  API_WorkQueueDelete(); 
extern int  API_WorkQueueFlush(); 
extern int  API_WorkQueueInsert(); 
extern int  API_WorkQueueStatus(); 
extern int  _Allocate_Event_Object(); 
extern int  _Free_Event_Object(); 
extern int  _Allocate_EventSet_Object(); 
extern int  _Free_EventSet_Object(); 
extern int  _Allocate_Heap_Object(); 
extern int  _Free_Heap_Object(); 
extern int  _List_Line_Add_Ahead(); 
extern int  _List_Line_Add_Left(); 
extern int  _List_Line_Add_Right(); 
extern int  _List_Line_Add_Tail(); 
extern int  _List_Line_Del(); 
extern int  _List_Ring_Add_Ahead(); 
extern int  _List_Ring_Add_Front(); 
extern int  _List_Ring_Add_Last(); 
extern int  _List_Ring_Del(); 
extern int  _Allocate_MsgQueue_Object(); 
extern int  _Free_MsgQueue_Object(); 
extern int  _Allocate_Partition_Object(); 
extern int  _Free_Partition_Object(); 
extern int  _Allocate_Rms_Object(); 
extern int  _Free_Rms_Object(); 
extern int  _Allocate_Tcb_Object(); 
extern int  _Free_Tcb_Object(); 
extern int  _Allocate_Timer_Object(); 
extern int  _Free_Timer_Object(); 
extern int  __resAddRawHook(); 
extern int  __resDelRawHook(); 
extern int  __resHandleIsGlobal(); 
extern int  __resHandleMakeGlobal(); 
extern int  __resHandleMakeLocal(); 
extern int  __resPidReclaim(); 
extern int  __resPidReclaimOnlyRaw(); 
extern int  __resThreadDelHook(); 
extern int  _resInit(); 
extern int  __resReclaimInit(); 
extern int  __resReclaimReq(); 
extern int  API_BacktracePrint(); 
extern int  API_BacktraceShow(); 
extern int  API_CPUUsageShow(); 
extern int  API_EventSetShow(); 
extern int  API_InterShow(); 
extern int  API_MsgQueueShow(); 
extern int  API_PartitionShow(); 
extern int  API_RegionShow(); 
extern int  API_RmsShow(); 
extern int  API_SemaphoreShow(); 
extern int  API_StackShow(); 
extern int  API_ThreadPendShow(); 
extern int  API_ThreadPendShowEx(); 
extern int  API_ThreadShow(); 
extern int  API_ThreadShowEx(); 
extern int  API_ThreadWjShow(); 
extern int  API_ThreadWjShowEx(); 
extern int  API_TimerShow(); 
extern int  API_TimeShow(); 
extern int  API_VmmAbortShow(); 
extern int  API_VmmPhysicalShow(); 
extern int  API_VmmVirtualShow(); 
extern int  API_ThreadCleanupPop(); 
extern int  API_ThreadCleanupPush(); 
extern int  API_ThreadCleanupPushEx(); 
extern int  API_ThreadCondAttrDestroy(); 
extern int  API_ThreadCondAttrGetPshared(); 
extern int  API_ThreadCondAttrInit(); 
extern int  API_ThreadCondAttrSetPshared(); 
extern int  API_ThreadCondBroadcast(); 
extern int  API_ThreadCondDestroy(); 
extern int  API_ThreadCondInit(); 
extern int  API_ThreadCondSignal(); 
extern int  API_ThreadCondWait(); 
extern int  API_ThreadOnce(); 
extern int  API_ThreadOnce2(); 
extern int  _Tree_Rb_Erase(); 
extern int  _Tree_Rb_Insert_Color(); 
extern int  __pageAllocate(); 
extern int  __pageAllocateAlign(); 
extern int  __pageCbInit(); 
extern int  __pageExpand(); 
extern int  __pageFindLink(); 
extern int  __pageFree(); 
extern int  __pageGetMinContinue(); 
extern int  __pageLink(); 
extern int  __pageMerge(); 
extern int  __pageSplit(); 
extern int  __pageTraversalLink(); 
extern int  __pageUnlink(); 
extern int  __pageZoneCreate(); 
extern int  __vmmGetCurCtx(); 
extern int  __vmmLibFlushTlb(); 
extern int  __vmmLibGetFlag(); 
extern int  __vmmLibPageMap(); 
extern int  __vmmLibPageMap2(); 
extern int  __vmmLibPrimaryInit(); 
extern int  __vmmLibSecondaryInit(); 
extern int  __vmmLibSetFlag(); 
extern int  __vmmLibVirtualOverlap(); 
extern int  __vmmLibVirtualToPhysical(); 
extern int  __vmmPhysicalCreate(); 
extern int  __vmmPhysicalGetKernelDesc(); 
extern int  __vmmPhysicalGetZone(); 
extern int  __vmmPhysicalPageAlloc(); 
extern int  __vmmPhysicalPageAllocAlign(); 
extern int  __vmmPhysicalPageAllocZone(); 
extern int  __vmmPhysicalPageClear(); 
extern int  __vmmPhysicalPageClearAll(); 
extern int  __vmmPhysicalPageClone(); 
extern int  __vmmPhysicalPageFaultCheck(); 
extern int  __vmmPhysicalPageFaultClear(); 
extern int  __vmmPhysicalPageFaultGuarder(); 
extern int  __vmmPhysicalPageFaultLimit(); 
extern int  __vmmPhysicalPageFlush(); 
extern int  __vmmPhysicalPageFlushAll(); 
extern int  __vmmPhysicalPageFree(); 
extern int  __vmmPhysicalPageFreeAll(); 
extern int  __vmmPhysicalPageGetMinContinue(); 
extern int  __vmmPhysicalPageInvalidate(); 
extern int  __vmmPhysicalPageInvalidateAll(); 
extern int  __vmmPhysicalPageRef(); 
extern int  __vmmPhysicalPageSetFlag(); 
extern int  __vmmPhysicalPageSetFlagAll(); 
extern int  __vmmVirDevPageAlloc(); 
extern int  __vmmVirDevPageAllocAlign(); 
extern int  __vmmVirDevPageFree(); 
extern int  __vmmVirtualCreate(); 
extern int  __vmmVirtualDesc(); 
extern int  __vmmVirtualIsInside(); 
extern int  __vmmVirtualPageAlloc(); 
extern int  __vmmVirtualPageAllocAlign(); 
extern int  __vmmVirtualPageFree(); 
extern int  __vmmVirtualSwitch(); 
extern int  API_VmmDmaAlloc(); 
extern int  API_VmmDmaAllocAlign(); 
extern int  API_VmmDmaAllocAlignWithFlags(); 
extern int  API_VmmDmaFree(); 
extern int  API_VmmGetFlag(); 
extern int  API_VmmGetLibBlock(); 
extern int  API_VmmLibAddPhyRam(); 
extern int  API_VmmLibPrimaryInit(); 
extern int  API_VmmLibSecondaryInit(); 
extern int  API_VmmMap(); 
extern int  API_VmmMmuDisable(); 
extern int  API_VmmMmuEnable(); 
extern int  API_VmmPageFaultGuarder(); 
extern int  API_VmmPageFaultLimit(); 
extern int  API_VmmPhyAlloc(); 
extern int  API_VmmPhyAllocAlign(); 
extern int  API_VmmPhyAllocEx(); 
extern int  API_VmmPhyFree(); 
extern int  API_VmmPhysicalKernelDesc(); 
extern int  API_VmmPhysicalToVirtual(); 
extern int  API_VmmSetFlag(); 
extern int  API_VmmVirtualIsInside(); 
extern int  API_VmmVirtualStatus(); 
extern int  API_VmmVirtualToPhysical(); 
extern int  API_VmmZoneStatus(); 
extern int  getpagesize(); 
extern int  __vmmAbortFakeSymbol(); 
extern int  API_VmmAbortIsr(); 
extern int  API_VmmAbortIsrEx(); 
extern int  API_VmmAbortReturn(); 
extern int  API_VmmAbortStatus(); 
extern int  __areaPhysicalInsertPage(); 
extern int  __areaPhysicalSearchPage(); 
extern int  __areaPhysicalSpaceInit(); 
extern int  __areaPhysicalSpaceTraversal(); 
extern int  __areaPhysicalUnlinkPage(); 
extern int  __areaVirtualInsertPage(); 
extern int  __areaVirtualSearchPage(); 
extern int  __areaVirtualSpaceInit(); 
extern int  __areaVirtualSpaceTraversal(); 
extern int  __areaVirtualUnlinkPage(); 
extern int  API_VmmIoRemap(); 
extern int  API_VmmIoRemap2(); 
extern int  API_VmmIoRemapEx(); 
extern int  API_VmmIoRemapEx2(); 
extern int  API_VmmIoRemapNocache(); 
extern int  API_VmmIoRemapNocache2(); 
extern int  API_VmmIoUnmap(); 
extern int  API_VmmExpandArea(); 
extern int  API_VmmFree(); 
extern int  API_VmmFreeArea(); 
extern int  API_VmmInvalidateArea(); 
extern int  API_VmmMalloc(); 
extern int  API_VmmMallocAlign(); 
extern int  API_VmmMallocArea(); 
extern int  API_VmmMallocAreaAlign(); 
extern int  API_VmmMallocAreaEx(); 
extern int  API_VmmMallocEx(); 
extern int  API_VmmMergeArea(); 
extern int  API_VmmMoveArea(); 
extern int  API_VmmPCountInArea(); 
extern int  API_VmmPreallocArea(); 
extern int  API_VmmRemapArea(); 
extern int  API_VmmRemapArea2(); 
extern int  API_VmmSetFiller(); 
extern int  API_VmmSetFindShare(); 
extern int  API_VmmShareArea(); 
extern int  API_VmmSplitArea(); 
extern int  __vmmMapInit(); 
extern int  API_VmmMmap(); 
extern int  API_VmmMmapPCount(); 
extern int  API_VmmMmapReclaim(); 
extern int  API_VmmMmapShow(); 
extern int  API_VmmMProtect(); 
extern int  API_VmmMremap(); 
extern int  API_VmmMsync(); 
extern int  API_VmmMunmap(); 
extern int  API_VmmStackAlloc(); 
extern int  API_VmmStackFree(); 
extern int  __vmmPageSwapIsNeedLoad(); 
extern int  __vmmPageSwapLoad(); 
extern int  __vmmPageSwapSwitch(); 
extern int  __assert(); 
extern int  abort(); 
extern int  abs(); 
extern int  aligned_free(); 
extern int  aligned_malloc(); 
extern int  asctime(); 
extern int  asctime_r(); 
extern int  atof(); 
extern int  atoi(); 
extern int  atol(); 
extern int  atoll(); 
extern int  bcmp(); 
extern int  bcopy(); 
extern int  brk(); 
extern int  bsearch(); 
extern int  bzero(); 
extern int  calloc(); 
extern int  clock(); 
extern int  clock_getcpuclockid(); 
extern int  clock_getres(); 
extern int  clock_gettime(); 
extern int  clock_nanosleep(); 
extern int  clock_settime(); 
extern int  ctime(); 
extern int  ctime_r(); 
extern int  difftime(); 
extern int  div(); 
extern int  drand48(); 
extern int  erand48(); 
extern int  ffs(); 
extern int  ffsll(); 
extern int  free(); 
extern int  getenv(); 
extern int  getenv_r(); 
extern int  gethrtime(); 
extern int  gethrvtime(); 
extern int  gettimeofday(); 
extern int  gmtime(); 
extern int  gmtime_r(); 
extern int  imaxabs(); 
extern int  imaxdiv(); 
extern int  index(); 
extern int  isalnum(); 
extern int  isalpha(); 
extern int  isascii(); 
extern int  isblank(); 
extern int  iscntrl(); 
extern int  isdigit(); 
extern int  isgraph(); 
extern int  isinf(); 
extern int  islower(); 
extern int  isnan(); 
extern int  isprint(); 
extern int  ispunct(); 
extern int  isspace(); 
extern int  isupper(); 
extern int  isxdigit(); 
extern int  itoa(); 
extern int  jrand48(); 
extern int  labs(); 
extern int  lcong48(); 
extern int  ldiv(); 
extern int  llabs(); 
extern int  lldiv(); 
extern int  localeconv(); 
extern int  localtime(); 
extern int  localtime_r(); 
extern int  lrand48(); 
extern int  malloc(); 
extern int  malloc_new(); 
extern int  mallocalign(); 
extern int  memalign(); 
extern int  memchr(); 
extern int  memcmp(); 
extern int  memcpy(); 
extern int  memmove(); 
extern int  mempcpy(); 
extern int  memrchr(); 
extern int  memset(); 
extern int  mktime(); 
extern int  mrand48(); 
extern int  nrand48(); 
extern int  posix_memalign(); 
extern int  putenv(); 
extern int  qsort(); 
extern int  rand(); 
extern int  rand_r(); 
extern int  random(); 
extern int  realloc(); 
extern int  rindex(); 
extern int  sbrk(); 
extern int  seed48(); 
extern int  setenv(); 
extern int  setlocale(); 
extern int  settimeofday(); 
extern int  srand(); 
extern int  srand48(); 
extern int  srandom(); 
extern int  stpcpy(); 
extern int  stpncpy(); 
extern int  strcasecmp(); 
extern int  strcasestr(); 
extern int  strcat(); 
extern int  strchr(); 
extern int  strchrnul(); 
extern int  strcmp(); 
extern int  strcoll(); 
extern int  strcpy(); 
extern int  strcspn(); 
extern int  strdup(); 
extern int  strerror(); 
extern int  strerror_r(); 
extern int  strftime(); 
extern int  stricmp(); 
extern int  strlcat(); 
extern int  strlcpy(); 
extern int  strlen(); 
extern int  strncasecmp(); 
extern int  strncat(); 
extern int  strncmp(); 
extern int  strncpy(); 
extern int  strndup(); 
extern int  strnlen(); 
extern int  strnset(); 
extern int  strpbrk(); 
extern int  strptime(); 
extern int  strrchr(); 
extern int  strsep(); 
extern int  strsignal(); 
extern int  strspn(); 
extern int  strstr(); 
extern int  strtod(); 
extern int  strtof(); 
extern int  strtoimax(); 
extern int  strtok(); 
extern int  strtok_r(); 
extern int  strtol(); 
extern int  strtold(); 
extern int  strtoll(); 
extern int  strtoul(); 
extern int  strtoull(); 
extern int  strtoumax(); 
extern int  strxfrm(); 
extern int  swab(); 
extern int  system(); 
extern int  timegm(); 
extern int  timelocal(); 
extern int  toascii(); 
extern int  tolower(); 
extern int  toupper(); 
extern int  tzset(); 
extern int  unsetenv(); 
extern int  wcstoimax(); 
extern int  wcstoumax(); 
extern int  xcalloc(); 
extern int  xmalloc(); 
extern int  xmallocalign(); 
extern int  xmemalign(); 
extern int  xrealloc(); 
extern int  xstrdup(); 
extern int  xstrndup(); 
extern int  __bcrypt(); 
extern int  __gensalt_blowfish(); 
extern int  bcrypt_gensalt(); 
extern int  __crypt_sha1(); 
extern int  __crypt_sha1_iterations(); 
extern int  crypt(); 
extern int  crypt_safe(); 
extern int  des_cipher(); 
extern int  des_setkey(); 
extern int  encrypt(); 
extern int  setkey(); 
extern int  __hmac_sha1(); 
extern int  __md5crypt(); 
extern int  __gensalt_md5(); 
extern int  __gensalt_new(); 
extern int  __gensalt_old(); 
extern int  __gensalt_sha1(); 
extern int  pw_gensalt(); 
extern int  __crypt_to64(); 
extern int  lib_isalnum(); 
extern int  lib_isalpha(); 
extern int  lib_isascii(); 
extern int  lib_isblank(); 
extern int  lib_iscntrl(); 
extern int  lib_isdigit(); 
extern int  lib_isgraph(); 
extern int  lib_islower(); 
extern int  lib_isprint(); 
extern int  lib_ispunct(); 
extern int  lib_isspace(); 
extern int  lib_isupper(); 
extern int  lib_isxdigit(); 
extern int  lib_toascii(); 
extern int  panic(); 
extern int  lib_isinf(); 
extern int  lib_isnan(); 
extern int  lib_imaxabs(); 
extern int  lib_imaxdiv(); 
extern int  lib_strtoimax(); 
extern int  lib_strtoumax(); 
extern int  lib_wcstoimax(); 
extern int  lib_wcstoumax(); 
extern int  __longjmpSetup(); 
extern int  __setjmpSetup(); 
extern int  asprintf(); 
extern int  vasprintf(); 
extern int  clearerr(); 
extern int  ctermid(); 
extern int  lib_cvtfloat(); 
extern int  fclose(); 
extern int  fclose_ex(); 
extern int  fdopen(); 
extern int  fdprintf(); 
extern int  fdscanf(); 
extern int  feof(); 
extern int  ferror(); 
extern int  __sflush(); 
extern int  fflush(); 
extern int  fgetc(); 
extern int  __slbexpand(); 
extern int  fgetln(); 
extern int  fgetpos(); 
extern int  fgetpos64(); 
extern int  fgets(); 
extern int  fileno(); 
extern int  __sfp(); 
extern int  __sinit(); 
extern int  _cleanup(); 
extern int  __sflags(); 
extern int  fopen(); 
extern int  fopen64(); 
extern int  fprintf(); 
extern int  fpurge(); 
extern int  fputc(); 
extern int  fputs(); 
extern int  fread(); 
extern int  freopen(); 
extern int  fscanf(); 
extern int  fseek(); 
extern int  fseeko(); 
extern int  fseeko64(); 
extern int  fsetpos(); 
extern int  fsetpos64(); 
extern int  ftell(); 
extern int  ftello(); 
extern int  ftello64(); 
extern int  funopen(); 
extern int  __sfvwrite(); 
extern int  _fwalk(); 
extern int  fwrite(); 
extern int  getc(); 
extern int  getchar(); 
extern int  gets(); 
extern int  getw(); 
extern int  __lib_delfile(); 
extern int  __lib_newfile(); 
extern int  __lib_stderr(); 
extern int  __lib_stdin(); 
extern int  __lib_stdout(); 
extern int  __smakebuf(); 
extern int  __swhatbuf(); 
extern int  _mktemp(); 
extern int  mkdtemp(); 
extern int  mkstemp(); 
extern int  mkstemps(); 
extern int  mktemp(); 
extern int  perror(); 
extern int  pclose(); 
extern int  popen(); 
extern int  printf(); 
extern int  putc(); 
extern int  putchar(); 
extern int  puts(); 
extern int  putw(); 
extern int  __srefill(); 
extern int  remove(); 
extern int  rewind(); 
extern int  __srget(); 
extern int  scanf(); 
extern int  setbuf(); 
extern int  setbuffer(); 
extern int  setlinebuf(); 
extern int  setvbuf(); 
extern int  bnprintf(); 
extern int  snprintf(); 
extern int  sprintf(); 
extern int  sscanf(); 
extern int  __sclose(); 
extern int  __sread(); 
extern int  __sseek(); 
extern int  __swrite(); 
extern int  tempnam(); 
extern int  tmpfile(); 
extern int  tmpnam(); 
extern int  tmpnam_r(); 
extern int  ungetc(); 
extern int  vfprintf(); 
extern int  __svfscanf(); 
extern int  vfscanf(); 
extern int  vprintf(); 
extern int  vscanf(); 
extern int  vbnprintf(); 
extern int  vsnprintf(); 
extern int  vsprintf(); 
extern int  vsscanf(); 
extern int  __swbuf(); 
extern int  __swsetup(); 
extern int  lib_abs(); 
extern int  lib_labs(); 
extern int  lib_llabs(); 
extern int  lib_getenv(); 
extern int  lib_getenv_r(); 
extern int  lib_putenv(); 
extern int  lib_setenv(); 
extern int  lib_unsetenv(); 
extern int  lib_div(); 
extern int  lib_ldiv(); 
extern int  lib_lldiv(); 
extern int  lib_calloc(); 
extern int  lib_free(); 
extern int  lib_malloc(); 
extern int  lib_malloc_new(); 
extern int  lib_mallocalign(); 
extern int  lib_memalign(); 
extern int  lib_posix_memalign(); 
extern int  lib_realloc(); 
extern int  lib_xcalloc(); 
extern int  lib_xmalloc(); 
extern int  lib_xmallocalign(); 
extern int  lib_xmemalign(); 
extern int  lib_xrealloc(); 
extern int  lib_drand48(); 
extern int  lib_erand48(); 
extern int  lib_jrand48(); 
extern int  lib_lcong48(); 
extern int  lib_lrand48(); 
extern int  lib_mrand48(); 
extern int  lib_nrand48(); 
extern int  lib_rand(); 
extern int  lib_rand_r(); 
extern int  lib_random(); 
extern int  lib_seed48(); 
extern int  lib_srand(); 
extern int  lib_srand48(); 
extern int  lib_srandom(); 
extern int  lib_bsearch(); 
extern int  lib_qsort(); 
extern int  lib_atof(); 
extern int  lib_atoi(); 
extern int  lib_atol(); 
extern int  lib_atoll(); 
extern int  lib_itoa(); 
extern int  lib_strtof(); 
extern int  lib_strtol(); 
extern int  lib_strtoll(); 
extern int  lib_strtoul(); 
extern int  lib_strtoull(); 
extern int  lib_strtod(); 
extern int  lib_strtold(); 
extern int  lib_abort(); 
extern int  lib_system(); 
extern int  lib_ffs(); 
extern int  lib_ffsll(); 
extern int  lib_index(); 
extern int  lib_memchr(); 
extern int  lib_bcmp(); 
extern int  lib_memcmp(); 
extern int  lib_memcpy(); 
extern int  lib_mempcpy(); 
extern int  lib_memrchr(); 
extern int  lib_bzero(); 
extern int  lib_memset(); 
extern int  lib_rindex(); 
extern int  lib_stpcpy(); 
extern int  lib_stpncpy(); 
extern int  lib_strcasecmp(); 
extern int  lib_strcasestr(); 
extern int  lib_strcat(); 
extern int  lib_strchrnul(); 
extern int  lib_strcmp(); 
extern int  lib_strcpy(); 
extern int  lib_strcspn(); 
extern int  lib_strdup(); 
extern int  lib_strerror(); 
extern int  lib_strerror_r(); 
extern int  lib_strftime(); 
extern int  lib_stricmp(); 
extern int  lib_strlen(); 
extern int  lib_strncasecmp(); 
extern int  lib_strlcat(); 
extern int  lib_strncat(); 
extern int  lib_strncmp(); 
extern int  lib_bcopy(); 
extern int  lib_strlcpy(); 
extern int  lib_strncpy(); 
extern int  lib_strndup(); 
extern int  lib_strnlen(); 
extern int  lib_strnset(); 
extern int  lib_strpbrk(); 
extern int  lib_strptime(); 
extern int  lib_strsep(); 
extern int  lib_strsignal(); 
extern int  lib_strspn(); 
extern int  lib_strstr(); 
extern int  lib_strtok(); 
extern int  lib_strtok_r(); 
extern int  lib_strxfrm(); 
extern int  lib_swab(); 
extern int  lib_tolower(); 
extern int  lib_toupper(); 
extern int  lib_xstrdup(); 
extern int  lib_xstrndup(); 
extern int  fstatvfs(); 
extern int  statvfs(); 
extern int  lib_asctime(); 
extern int  lib_asctime_r(); 
extern int  lib_clock(); 
extern int  lib_clock_getcpuclockid(); 
extern int  lib_clock_getres(); 
extern int  lib_clock_gettime(); 
extern int  lib_clock_nanosleep(); 
extern int  lib_clock_settime(); 
extern int  lib_ctime(); 
extern int  lib_ctime_r(); 
extern int  lib_gettimeofday(); 
extern int  lib_settimeofday(); 
extern int  lib_difftime(); 
extern int  __daysSinceEpoch(); 
extern int  __gettime(); 
extern int  __julday(); 
extern int  lib_gmtime(); 
extern int  lib_gmtime_r(); 
extern int  lib_gethrtime(); 
extern int  lib_gethrvtime(); 
extern int  lib_localtime(); 
extern int  lib_localtime_r(); 
extern int  lib_mktime(); 
extern int  lib_timegm(); 
extern int  lib_time(); 
extern int  lib_timelocal(); 
extern int  time(); 
extern int  lib_tzset(); 
extern int  endgrent(); 
extern int  endpwent(); 
extern int  getgrent(); 
extern int  getgrgid(); 
extern int  getgrgid_r(); 
extern int  getgrnam(); 
extern int  getgrnam_r(); 
extern int  getpwent(); 
extern int  getpwnam(); 
extern int  getpwnam_r(); 
extern int  getpwuid(); 
extern int  getpwuid_r(); 
extern int  initgroups(); 
extern int  scangr(); 
extern int  scanInt(); 
extern int  scanpw(); 
extern int  scanString(); 
extern int  setgrent(); 
extern int  setpwent(); 
extern int  endspent(); 
extern int  fgetspent(); 
extern int  fgetspent_r(); 
extern int  getpass(); 
extern int  getpass_r(); 
extern int  getspent(); 
extern int  getspent_r(); 
extern int  getspnam(); 
extern int  getspnam_r(); 
extern int  passwdcheck(); 
extern int  putspent(); 
extern int  scansp(); 
extern int  setspent(); 
extern int  userlogin(); 
extern int  user_db_gadd(); 
extern int  user_db_gdel(); 
extern int  user_db_gget(); 
extern int  user_db_pmod(); 
extern int  user_db_uadd(); 
extern int  user_db_udel(); 
extern int  user_db_uget(); 
extern int  user_db_umod(); 
extern int  btowc(); 
extern int  fgetwc(); 
extern int  fgetws(); 
extern int  fputwc(); 
extern int  fputws(); 
extern int  fwide(); 
extern int  fwprintf(); 
extern int  fwscanf(); 
extern int  getwc(); 
extern int  getwchar(); 
extern int  iswalnum(); 
extern int  iswalpha(); 
extern int  iswcntrl(); 
extern int  iswctype(); 
extern int  iswdigit(); 
extern int  iswgraph(); 
extern int  iswlower(); 
extern int  iswprint(); 
extern int  iswpunct(); 
extern int  iswspace(); 
extern int  iswupper(); 
extern int  iswxdigit(); 
extern int  mblen(); 
extern int  mbrlen(); 
extern int  mbrtowc(); 
extern int  mbsinit(); 
extern int  mbsrtowcs(); 
extern int  mbstowcs(); 
extern int  mbtowc(); 
extern int  putwc(); 
extern int  putwchar(); 
extern int  swprintf(); 
extern int  swscanf(); 
extern int  towlower(); 
extern int  towupper(); 
extern int  ungetwc(); 
extern int  vfwprintf(); 
extern int  vfwscanf(); 
extern int  vswprintf(); 
extern int  vswscanf(); 
extern int  vwprintf(); 
extern int  vwscanf(); 
extern int  wcrtomb(); 
extern int  wcscoll(); 
extern int  wcsftime(); 
extern int  wcsrtombs(); 
extern int  wcstod(); 
extern int  wcstof(); 
extern int  wcstol(); 
extern int  wcstold(); 
extern int  wcstoll(); 
extern int  wcstombs(); 
extern int  wcstoul(); 
extern int  wcstoull(); 
extern int  wcswidth(); 
extern int  wcsxfrm(); 
extern int  wctob(); 
extern int  wctomb(); 
extern int  wctype(); 
extern int  wcwidth(); 
extern int  wprintf(); 
extern int  wscanf(); 
extern int  wcscasecmp(); 
extern int  wcscat(); 
extern int  wcschr(); 
extern int  wcscmp(); 
extern int  wcscpy(); 
extern int  wcscspn(); 
extern int  wcsdup(); 
extern int  wcslcat(); 
extern int  wcslcpy(); 
extern int  wcslen(); 
extern int  wcsncasecmp(); 
extern int  wcsncat(); 
extern int  wcsncmp(); 
extern int  wcsncpy(); 
extern int  wcspbrk(); 
extern int  wcsrchr(); 
extern int  wcsspn(); 
extern int  wcsstr(); 
extern int  wcstok(); 
extern int  wcswcs(); 
extern int  wmemchr(); 
extern int  wmemcmp(); 
extern int  wmemcpy(); 
extern int  wmemmove(); 
extern int  wmemset(); 
extern int  __sync_synchronize(); 
extern int  __getreent(); 
extern int  lib_nlreent_init(); 
extern int  lib_nlreent_static(); 
extern int  lib_nlreent_stdfile(); 
extern int  __elfListLoad(); 
extern int  __elfListUnload(); 
extern int  __elfStatus(); 
extern int  API_ModuleFinish(); 
extern int  API_ModuleGcov(); 
extern int  API_ModuleGetName(); 
extern int  API_ModuleGlobal(); 
extern int  API_ModuleLoad(); 
extern int  API_ModuleLoadEx(); 
extern int  API_ModuleProcSym(); 
extern int  API_ModuleShareConfig(); 
extern int  API_ModuleShareRefresh(); 
extern int  API_ModuleStatus(); 
extern int  API_ModuleSym(); 
extern int  API_ModuleTerminal(); 
extern int  API_ModuleUnload(); 
extern int  moduleLoadSub(); 
extern int  vprocGetAffinity(); 
extern int  vprocSetAffinity(); 
extern int  __processStart(); 
extern int  __spawnArgActionDup(); 
extern int  __spawnArgCreate(); 
extern int  __spawnArgFree(); 
extern int  __spawnArgProc(); 
extern int  execl(); 
extern int  execle(); 
extern int  execlp(); 
extern int  execv(); 
extern int  execve(); 
extern int  execvp(); 
extern int  execvpe(); 
extern int  spawnl(); 
extern int  spawnle(); 
extern int  spawnlp(); 
extern int  spawnv(); 
extern int  spawnve(); 
extern int  spawnvp(); 
extern int  spawnvpe(); 
extern int  vprocIoFileDeinit(); 
extern int  vprocIoFileDescGet(); 
extern int  vprocIoFileDup(); 
extern int  vprocIoFileDup2(); 
extern int  vprocIoFileDupFrom(); 
extern int  vprocIoFileGet(); 
extern int  vprocIoFileGetEx(); 
extern int  vprocIoFileInit(); 
extern int  vprocIoFileRefDec(); 
extern int  vprocIoFileRefDecArryByPid(); 
extern int  vprocIoFileRefDecByPid(); 
extern int  vprocIoFileRefGet(); 
extern int  vprocIoFileRefGetByPid(); 
extern int  vprocIoFileRefInc(); 
extern int  vprocIoFileRefIncArryByPid(); 
extern int  vprocIoFileRefIncByPid(); 
extern int  vprocIoReclaim(); 
extern int  __ldFree(); 
extern int  __ldFreeArea(); 
extern int  __ldMalloc(); 
extern int  __ldMallocAlign(); 
extern int  __ldMallocArea(); 
extern int  __ldMallocAreaAlign(); 
extern int  __ldMmap(); 
extern int  __ldProtect(); 
extern int  __ldShare(); 
extern int  __ldShareAbort(); 
extern int  __ldShareConfig(); 
extern int  vprocProcAdd(); 
extern int  vprocProcDelete(); 
extern int  __ldGetFilePath(); 
extern int  __ldPathIsFile(); 
extern int  API_LoaderInit(); 
extern int  __moduleDeleteAllSymbol(); 
extern int  __moduleExportSymbol(); 
extern int  __moduleFindKernelSymHook(); 
extern int  __moduleFindSym(); 
extern int  __moduleSymbolBufferSize(); 
extern int  __moduleSymGetValue(); 
extern int  __moduleTraverseKernelSymHook(); 
extern int  __moduleTraverseSym(); 
extern int  __moduleVerifyVersion(); 
extern int  vprocDebugContinue(); 
extern int  vprocDebugStop(); 
extern int  vprocDebugThreadContinue(); 
extern int  vprocDebugThreadGet(); 
extern int  vprocDebugThreadStop(); 
extern int  __moduleVpPatchFini(); 
extern int  __moduleVpPatchInit(); 
extern int  __moduleVpPatchVersion(); 
extern int  __moduleVpPatchVmem(); 
extern int  API_ModuleAddr(); 
extern int  API_ModuleGetBase(); 
extern int  API_ModulePid(); 
extern int  API_ModuleRun(); 
extern int  API_ModuleRunEx(); 
extern int  API_ModuleTimes(); 
extern int  vprocCreate(); 
extern int  vprocDebugFlagsGet(); 
extern int  vprocDebugFlagsSet(); 
extern int  vprocDestroy(); 
extern int  vprocDetach(); 
extern int  vprocExit(); 
extern int  vprocExitModeGet(); 
extern int  vprocExitModeSet(); 
extern int  vprocExitNotDestroy(); 
extern int  vprocFindProc(); 
extern int  vprocGet(); 
extern int  vprocGetCur(); 
extern int  vprocGetFather(); 
extern int  vprocGetGroup(); 
extern int  vprocGetMain(); 
extern int  vprocGetModsInfo(); 
extern int  vprocGetPath(); 
extern int  vprocGetPidByTcb(); 
extern int  vprocGetPidByTcbNoLock(); 
extern int  vprocGetPidByThread(); 
extern int  vprocIoEnvGet(); 
extern int  vprocIsMainThread(); 
extern int  vprocKillPrepare(); 
extern int  vprocMainThread(); 
extern int  vprocNotifyParent(); 
extern int  vprocReclaim(); 
extern int  vprocRun(); 
extern int  vprocSetCur(); 
extern int  vprocSetGroup(); 
extern int  vprocSetImmediatelyTerm(); 
extern int  vprocThreadExitHook(); 
extern int  vprocTickHook(); 
extern int  vprocStackAlloc(); 
extern int  vprocStackFree(); 
extern int  vprocListGet(); 
extern int  vprocMemInfo(); 
extern int  vprocMemInfoNoLock(); 
extern int  vprocThreadAdd(); 
extern int  vprocThreadAffinity(); 
extern int  vprocThreadDelete(); 
extern int  vprocThreadKill(); 
extern int  vprocThreadNum(); 
extern int  vprocThreadSigaction(); 
extern int  vprocThreadTraversal(); 
extern int  vprocGetitimer(); 
extern int  vprocItimerEachHook(); 
extern int  vprocItimerMainHook(); 
extern int  vprocSetitimer(); 
extern int  daemon(); 
extern int  detach(); 
extern int  fork(); 
extern int  getpgid(); 
extern int  getpgrp(); 
extern int  getpid(); 
extern int  getppid(); 
extern int  isbrother(); 
extern int  ischild(); 
extern int  issetugid(); 
extern int  reclaimchild(); 
extern int  setpgid(); 
extern int  setpgrp(); 
extern int  setsid(); 
extern int  wait(); 
extern int  wait3(); 
extern int  wait4(); 
extern int  waitid(); 
extern int  waitpid(); 
extern int  API_PortCreate(); 
extern int  API_PortDelete(); 
extern int  API_PortExToIn(); 
extern int  API_PortGetName(); 
extern int  API_PortInToEx(); 
extern int  _Allocate_Dpma_Object(); 
extern int  _DpmaInit(); 
extern int  _Free_Dpma_Object(); 
extern int  _mpiInit(); 
extern int  _endhosthtent(); 
extern int  _gethostbyhtaddr(); 
extern int  _gethostbyhtname(); 
extern int  _map_v4v6_address(); 
extern int  _sethosthtent(); 
extern int  endhostent(); 
extern int  gethostent(); 
extern int  gethostent_r(); 
extern int  gethostent2_r(); 
extern int  sethostent(); 
extern int  freeifaddrs(); 
extern int  getifaddrs(); 
extern int  getprotobynumber(); 
extern int  getprotobynumber_r(); 
extern int  getprotobyname_static(); 
extern int  getprotobynumber_static(); 
extern int  endprotoent(); 
extern int  getprotoent(); 
extern int  getprotoent_r(); 
extern int  setprotoent(); 
extern int  getprotobyname(); 
extern int  getprotobyname_r(); 
extern int  getservbyname(); 
extern int  getservbyname_r(); 
extern int  getservbyport(); 
extern int  getservbyport_r(); 
extern int  endservent(); 
extern int  getservent(); 
extern int  getservent_r(); 
extern int  setservent(); 
extern int  inet_ntop(); 
extern int  inet_pton(); 
extern int  __ifIoctlArp(); 
extern int  _netBondingInit(); 
extern int  _netBridgeInit(); 
extern int  htonl(); 
extern int  htons(); 
extern int  inet_addr(); 
extern int  inet_aton(); 
extern int  inet_ntoa(); 
extern int  inet_ntoa_r(); 
extern int  inet6_aton(); 
extern int  inet6_ntoa(); 
extern int  inet6_ntoa_r(); 
extern int  ip_gw_hook(); 
extern int  ip_input_hook(); 
extern int  ip_route_default_hook(); 
extern int  ip_route_src_hook(); 
extern int  ip6_gw_hook(); 
extern int  ip6_input_hook(); 
extern int  ip6_route_default_hook(); 
extern int  ip6_route_src_hook(); 
extern int  link_input_hook(); 
extern int  link_output_hook(); 
extern int  ntohl(); 
extern int  ntohs(); 
extern int  sio_open(); 
extern int  sio_read(); 
extern int  sio_read_abort(); 
extern int  sio_recv(); 
extern int  sio_send(); 
extern int  sio_tryread(); 
extern int  sio_write(); 
extern int  sys_arch_mbox_fetch(); 
extern int  sys_arch_mbox_tryfetch(); 
extern int  sys_arch_msleep(); 
extern int  sys_arch_protect(); 
extern int  sys_arch_sem_wait(); 
extern int  sys_arch_unprotect(); 
extern int  sys_assert_print(); 
extern int  sys_error_print(); 
extern int  sys_init(); 
extern int  sys_jiffies(); 
extern int  sys_mbox_free(); 
extern int  sys_mbox_new(); 
extern int  sys_mbox_post(); 
extern int  sys_mbox_post_prio(); 
extern int  sys_mbox_set_invalid(); 
extern int  sys_mbox_trypost(); 
extern int  sys_mbox_trypost_fromisr(); 
extern int  sys_mbox_trypost_prio(); 
extern int  sys_mbox_trypost_stat(); 
extern int  sys_mbox_valid(); 
extern int  sys_mutex_free(); 
extern int  sys_mutex_lock(); 
extern int  sys_mutex_new(); 
extern int  sys_mutex_set_invalid(); 
extern int  sys_mutex_unlock(); 
extern int  sys_mutex_valid(); 
extern int  sys_now(); 
extern int  sys_sem_free(); 
extern int  sys_sem_new(); 
extern int  sys_sem_set_invalid(); 
extern int  sys_sem_signal(); 
extern int  sys_sem_valid(); 
extern int  sys_thread_hostent(); 
extern int  sys_thread_new(); 
extern int  sys_thread_sem_get(); 
extern int  __fcIoctlInet(); 
extern int  __tshellFlowctlInit(); 
extern int  lwip_ip_hook(); 
extern int  lwip_ip_nat_hook(); 
extern int  lwip_ip_route_hook(); 
extern int  net_ip_hook_add(); 
extern int  net_ip_hook_delete(); 
extern int  net_ip_hook_isadd(); 
extern int  net_ip_hook_nat_add(); 
extern int  net_ip_hook_nat_delete(); 
extern int  net_ip_hook_nat_isadd(); 
extern int  net_ip_hook_netif_get_flags(); 
extern int  net_ip_hook_netif_get_gw(); 
extern int  net_ip_hook_netif_get_hwaddr(); 
extern int  net_ip_hook_netif_get_index(); 
extern int  net_ip_hook_netif_get_ip6addr(); 
extern int  net_ip_hook_netif_get_ipaddr(); 
extern int  net_ip_hook_netif_get_linkspeed(); 
extern int  net_ip_hook_netif_get_name(); 
extern int  net_ip_hook_netif_get_netdev(); 
extern int  net_ip_hook_netif_get_netmask(); 
extern int  net_ip_hook_netif_get_type(); 
extern int  net_ip_hook_pbuf_set_ifout(); 
extern int  net_ip_hook_route_add(); 
extern int  net_ip_hook_route_delete(); 
extern int  net_ip_hook_route_isadd(); 
extern int  if_down(); 
extern int  if_freenameindex(); 
extern int  if_get_dhcp(); 
extern int  if_get_dhcp6(); 
extern int  if_indextoname(); 
extern int  if_islink(); 
extern int  if_isup(); 
extern int  if_list_init(); 
extern int  if_list_lock(); 
extern int  if_list_unlock(); 
extern int  if_nameindex(); 
extern int  if_nameindex_bufsize(); 
extern int  if_nameindex_rnp(); 
extern int  if_nametoindex(); 
extern int  if_set_dhcp(); 
extern int  if_set_dhcp6(); 
extern int  if_up(); 
extern int  __ifIoctlInet(); 
extern int  __ifIoctlLp(); 
extern int  __ifIoctlPacket(); 
extern int  __ifIoctlPrivate(); 
extern int  __ifIoctlWireless(); 
extern int  __ifSubIoctlStats(); 
extern int  if_param_getaodv(); 
extern int  if_param_getdefault(); 
extern int  if_param_getdhcp(); 
extern int  if_param_getdhcp6(); 
extern int  if_param_getenable(); 
extern int  if_param_getgw(); 
extern int  if_param_getgw_6(); 
extern int  if_param_getinaddr(); 
extern int  if_param_getingw(); 
extern int  if_param_getinnetmask(); 
extern int  if_param_getipaddr(); 
extern int  if_param_getipaddr_6(); 
extern int  if_param_getmac(); 
extern int  if_param_getmgw(); 
extern int  if_param_getmipaddr(); 
extern int  if_param_getmnetmask(); 
extern int  if_param_getnetmask(); 
extern int  if_param_gettxqblock(); 
extern int  if_param_gettxqueue(); 
extern int  if_param_ipv6autocfg(); 
extern int  if_param_load(); 
extern int  if_param_syncdns(); 
extern int  if_param_tcpackfreq(); 
extern int  if_param_tcpwnd(); 
extern int  if_param_unload(); 
extern int  _netJobqueueInit(); 
extern int  API_NetJobAdd(); 
extern int  API_NetJobAddEx(); 
extern int  API_NetJobDelete(); 
extern int  API_NetJobDeleteEx(); 
extern int  API_NetJobGetLost(); 
extern int  API_LoginBlAdd(); 
extern int  API_LoginBlDelete(); 
extern int  API_LoginBlShow(); 
extern int  API_LoginWlAdd(); 
extern int  API_LoginWlDelete(); 
extern int  API_LoginWlShow(); 
extern int  loginbl_input_hook(); 
extern int  __mrtIoctlInet(); 
extern int  netif_callback_init(); 
extern int  netif_get_flags(); 
extern int  netif_get_priv_flags(); 
extern int  netif_set_maddr_hook(); 
extern int  netif_set_maddr6_hook(); 
extern int  __tshellNetstatGroup(); 
extern int  __tshellNetstatIf(); 
extern int  __tshellNetstatKeepalive(); 
extern int  __tshellNetstatPacket(); 
extern int  __tshellNetstatRaw(); 
extern int  __tshellNetstatStat(); 
extern int  __tshellNetstatTcp(); 
extern int  __tshellNetstatTcpListen(); 
extern int  __tshellNetstatUdp(); 
extern int  __tshellNetstatUnix(); 
extern int  __qosIoctlInet(); 
extern int  __tshellRoute(); 
extern int  __tshellRouteInit(); 
extern int  __tshellSrouteInit(); 
extern int  route_add(); 
extern int  route_change(); 
extern int  route_delete(); 
extern int  route_get(); 
extern int  route_list(); 
extern int  __rtIoctlInet(); 
extern int  __tshellNetInit(); 
extern int  __tshellNet6Init(); 
extern int  __socketEnotify(); 
extern int  __socketEnotify2(); 
extern int  __socketInit(); 
extern int  __socketReset(); 
extern int  accept(); 
extern int  accept4(); 
extern int  bind(); 
extern int  connect(); 
extern int  freeaddrinfo(); 
extern int  get_dns_server_info_4(); 
extern int  get_dns_server_info_6(); 
extern int  getaddrinfo(); 
extern int  gethostbyaddr(); 
extern int  gethostbyaddr_r(); 
extern int  gethostbyname(); 
extern int  gethostbyname_r(); 
extern int  getpeername(); 
extern int  getsockname(); 
extern int  getsockopt(); 
extern int  listen(); 
extern int  recv(); 
extern int  recvfrom(); 
extern int  recvmsg(); 
extern int  send(); 
extern int  sendmsg(); 
extern int  sendto(); 
extern int  setsockopt(); 
extern int  shutdown(); 
extern int  socket(); 
extern int  socketpair(); 
extern int  __srtIoctlInet(); 
extern int  API_NetInit(); 
extern int  API_NetSnmpInit(); 
extern int  __ifIoctlVlan(); 
extern int  _netVndInit(); 
extern int  srt_sentry_to_srtentry(); 
extern int  srt_srtentry_to_sentry(); 
extern int  srt_add_entry(); 
extern int  srt_delete_entry(); 
extern int  srt_find_entry(); 
extern int  srt_netif_add_hook(); 
extern int  srt_netif_remove_hook(); 
extern int  srt_route_default_hook(); 
extern int  srt_route_search_hook(); 
extern int  srt_search_entry(); 
extern int  srt_total_entry(); 
extern int  srt_traversal_entry(); 
extern int  srt6_sentry_to_srtentry(); 
extern int  srt6_srtentry_to_sentry(); 
extern int  srt6_add_entry(); 
extern int  srt6_delete_entry(); 
extern int  srt6_find_entry(); 
extern int  srt6_netif_add_hook(); 
extern int  srt6_netif_remove_hook(); 
extern int  srt6_route_default_hook(); 
extern int  srt6_route_search_hook(); 
extern int  srt6_search_entry(); 
extern int  srt6_total_entry(); 
extern int  srt6_traversal_entry(); 
extern int  netbd_add(); 
extern int  netbd_add_arp(); 
extern int  netbd_add_dev(); 
extern int  netbd_change(); 
extern int  netbd_delete(); 
extern int  netbd_delete_arp(); 
extern int  netbd_delete_dev(); 
extern int  netbd_master_dev(); 
extern int  netbd_show_dev(); 
extern int  netbd_sub_delete_hook(); 
extern int  netbr_add(); 
extern int  netbr_add_dev(); 
extern int  netbr_delete(); 
extern int  netbr_delete_dev(); 
extern int  netbr_flush_cache(); 
extern int  netbr_show_dev(); 
extern int  netbr_sub_delete_hook(); 
extern int  _netEventInit(); 
extern int  netEventIfAdd(); 
extern int  netEventIfAddr(); 
extern int  netEventIfAddrConflict(); 
extern int  netEventIfAuthFail(); 
extern int  netEventIfDown(); 
extern int  netEventIfLink(); 
extern int  netEventIfPppExt(); 
extern int  netEventIfRemove(); 
extern int  netEventIfUnlink(); 
extern int  netEventIfUp(); 
extern int  netEventIfWlExt(); 
extern int  netEventIfWlExt2(); 
extern int  netEventIfWlQual(); 
extern int  netEventIfWlScan(); 
extern int  fcnet_netif_attach(); 
extern int  fcnet_netif_detach(); 
extern int  tlsf_mem_calloc(); 
extern int  tlsf_mem_create(); 
extern int  tlsf_mem_free(); 
extern int  tlsf_mem_malloc(); 
extern int  tlsf_add_pool(); 
extern int  tlsf_align_size(); 
extern int  tlsf_alloc_overhead(); 
extern int  tlsf_block_size(); 
extern int  tlsf_block_size_max(); 
extern int  tlsf_block_size_min(); 
extern int  tlsf_check(); 
extern int  tlsf_check_pool(); 
extern int  tlsf_create(); 
extern int  tlsf_create_with_pool(); 
extern int  tlsf_destroy(); 
extern int  tlsf_free(); 
extern int  tlsf_get_pool(); 
extern int  tlsf_malloc(); 
extern int  tlsf_memalign(); 
extern int  tlsf_pool_overhead(); 
extern int  tlsf_realloc(); 
extern int  tlsf_remove_pool(); 
extern int  tlsf_size(); 
extern int  tlsf_walk_pool(); 
extern int  ip4_mrt_clean(); 
extern int  ip4_mrt_forward(); 
extern int  ip4_mrt_getsockopt(); 
extern int  ip4_mrt_if_detach(); 
extern int  ip4_mrt_ioctl(); 
extern int  ip4_mrt_ipip_input(); 
extern int  ip4_mrt_is_on(); 
extern int  ip4_mrt_legal_vif_num(); 
extern int  ip4_mrt_mcast_src(); 
extern int  ip4_mrt_mrtstat(); 
extern int  ip4_mrt_pim_input(); 
extern int  ip4_mrt_pimstat(); 
extern int  ip4_mrt_rtdetq_alloc(); 
extern int  ip4_mrt_setsockopt(); 
extern int  ip4_mrt_traversal_mfc(); 
extern int  ip6_mrt_clean(); 
extern int  ip6_mrt_forward(); 
extern int  ip6_mrt_getsockopt(); 
extern int  ip6_mrt_if_detach(); 
extern int  ip6_mrt_ioctl(); 
extern int  ip6_mrt_is_on(); 
extern int  ip6_mrt_legal_mif_num(); 
extern int  ip6_mrt_mrt6stat(); 
extern int  ip6_mrt_pim_input(); 
extern int  ip6_mrt_pim6stat(); 
extern int  ip6_mrt_rtdetq_alloc(); 
extern int  ip6_mrt_setsockopt(); 
extern int  ip6_mrt_traversal_mfc(); 
extern int  crc16(); 
extern int  crc32_le(); 
extern int  netdev_desc_helper_create(); 
extern int  netdev_desc_helper_delete(); 
extern int  netdev_desc_rx_input(); 
extern int  netdev_desc_rx_input_offset(); 
extern int  netdev_desc_rx_refill(); 
extern int  netdev_desc_rx_refill_res(); 
extern int  netdev_desc_tx_clean(); 
extern int  netdev_desc_tx_prepare(); 
extern int  netdev_mipif_add(); 
extern int  netdev_mipif_clean(); 
extern int  netdev_mipif_delete(); 
extern int  netdev_mipif_hwaddr(); 
extern int  netdev_mipif_search(); 
extern int  netdev_mipif_tcpupd(); 
extern int  netdev_mipif_update(); 
extern int  netdev_txq_disable(); 
extern int  netdev_txq_enable(); 
extern int  netdev_txq_isenable(); 
extern int  netdev_txq_length(); 
extern int  netdev_txq_transmit(); 
extern int  netdev_add(); 
extern int  netdev_delete(); 
extern int  netdev_find_by_devname(); 
extern int  netdev_find_by_ifname(); 
extern int  netdev_find_by_index(); 
extern int  netdev_firewall(); 
extern int  netdev_foreache(); 
extern int  netdev_get_ar_hdr(); 
extern int  netdev_get_linkup(); 
extern int  netdev_get_tcpaf(); 
extern int  netdev_get_tcpwnd(); 
extern int  netdev_ifname(); 
extern int  netdev_index(); 
extern int  netdev_linkinfo_chkerr_inc(); 
extern int  netdev_linkinfo_drop_inc(); 
extern int  netdev_linkinfo_err_inc(); 
extern int  netdev_linkinfo_lenerr_inc(); 
extern int  netdev_linkinfo_memerr_inc(); 
extern int  netdev_linkinfo_recv_inc(); 
extern int  netdev_linkinfo_xmit_inc(); 
extern int  netdev_linkup_wd_add(); 
extern int  netdev_linkup_wd_delete(); 
extern int  netdev_macfilter_add(); 
extern int  netdev_macfilter_count(); 
extern int  netdev_macfilter_delete(); 
extern int  netdev_macfilter_isempty(); 
extern int  netdev_notify(); 
extern int  netdev_notify_clear(); 
extern int  netdev_notify_clear_ex(); 
extern int  netdev_notify_clear_ex_arg(); 
extern int  netdev_notify_ex(); 
extern int  netdev_notify_ex_arg(); 
extern int  netdev_pbuf_alloc(); 
extern int  netdev_pbuf_alloc_raw(); 
extern int  netdev_pbuf_can_ref(); 
extern int  netdev_pbuf_data(); 
extern int  netdev_pbuf_ethhdr(); 
extern int  netdev_pbuf_free(); 
extern int  netdev_pbuf_ip6hdr(); 
extern int  netdev_pbuf_iphdr(); 
extern int  netdev_pbuf_link(); 
extern int  netdev_pbuf_pull(); 
extern int  netdev_pbuf_push(); 
extern int  netdev_pbuf_tcphdr(); 
extern int  netdev_pbuf_trunc(); 
extern int  netdev_pbuf_udphdr(); 
extern int  netdev_pbuf_vlan_id(); 
extern int  netdev_pbuf_vlan_present(); 
extern int  netdev_pbuf_vlan_proto(); 
extern int  netdev_pbuf_vlanhdr(); 
extern int  netdev_poll_disable(); 
extern int  netdev_poll_enable(); 
extern int  netdev_poll_svc(); 
extern int  netdev_qoshook(); 
extern int  netdev_set_ar_hdr(); 
extern int  netdev_set_linkup(); 
extern int  netdev_set_tcpaf(); 
extern int  netdev_set_tcpwnd(); 
extern int  netdev_statinfo_collisions_inc(); 
extern int  netdev_statinfo_discards_inc(); 
extern int  netdev_statinfo_errors_inc(); 
extern int  netdev_statinfo_mcasts_inc(); 
extern int  netdev_statinfo_total_add(); 
extern int  netdev_statinfo_ucasts_inc(); 
extern int  netdev_zc_pbuf_alloc(); 
extern int  netdev_zc_pbuf_alloc_res(); 
extern int  netdev_zc_pbuf_free(); 
extern int  netdev_zc_pbuf_pool_create(); 
extern int  netdev_zc_pbuf_pool_delete(); 
extern int  netdev_zc_pbuf_stat(); 
extern int  vnetdev_add(); 
extern int  vnetdev_bufsize(); 
extern int  vnetdev_checksum(); 
extern int  vnetdev_delete(); 
extern int  vnetdev_flush(); 
extern int  vnetdev_get(); 
extern int  vnetdev_linkup(); 
extern int  vnetdev_maxplen(); 
extern int  vnetdev_mtu(); 
extern int  vnetdev_nrbytes(); 
extern int  vnetdev_nread(); 
extern int  vnetdev_put(); 
extern int  __packet_have_event(); 
extern int  __packet_set_sockfile(); 
extern int  packet_accept(); 
extern int  packet_bind(); 
extern int  packet_close(); 
extern int  packet_connect(); 
extern int  packet_getpeername(); 
extern int  packet_getsockname(); 
extern int  packet_getsockopt(); 
extern int  packet_init(); 
extern int  packet_ioctl(); 
extern int  packet_link_input(); 
extern int  packet_listen(); 
extern int  packet_mmap(); 
extern int  packet_recv(); 
extern int  packet_recvfrom(); 
extern int  packet_recvmsg(); 
extern int  packet_send(); 
extern int  packet_sendmsg(); 
extern int  packet_sendto(); 
extern int  packet_setsockopt(); 
extern int  packet_shutdown(); 
extern int  packet_socket(); 
extern int  packet_traversal(); 
extern int  packet_unmap(); 
extern int  __packetEthDgramSendto(); 
extern int  __packetEthHeaderInfo(); 
extern int  __packetEthHeaderInfo2(); 
extern int  __packetEthIfInfo(); 
extern int  __packetEthRawSendto(); 
extern int  __procFsNetInit(); 
extern int  __route_have_event(); 
extern int  __route_set_sockfile(); 
extern int  route_close(); 
extern int  route_getsockopt(); 
extern int  route_hook_maddr_ipv4(); 
extern int  route_hook_maddr_ipv6(); 
extern int  route_hook_netif_ann(); 
extern int  route_hook_netif_ipv4(); 
extern int  route_hook_netif_ipv6(); 
extern int  route_hook_netif_updown(); 
extern int  route_hook_rt_ipv4(); 
extern int  route_hook_rt_ipv6(); 
extern int  route_init(); 
extern int  route_ioctl(); 
extern int  route_recv(); 
extern int  route_recvmsg(); 
extern int  route_send(); 
extern int  route_sendmsg(); 
extern int  route_setsockopt(); 
extern int  route_shutdown(); 
extern int  route_socket(); 
extern int  rt_add_entry(); 
extern int  rt_change_default(); 
extern int  rt_delete_default(); 
extern int  rt_delete_entry(); 
extern int  rt_find_entry(); 
extern int  rt_get_default(); 
extern int  rt_netif_add_hook(); 
extern int  rt_netif_linkstat_hook(); 
extern int  rt_netif_remove_hook(); 
extern int  rt_route_gateway_hook(); 
extern int  rt_route_search_hook(); 
extern int  rt_total_entry(); 
extern int  rt_traversal_entry(); 
extern int  rt_build_sockaddr_dl(); 
extern int  rt_entry_to_msghdr(); 
extern int  rt_entry_to_rtentry(); 
extern int  rt_msghdr_to_entry(); 
extern int  rt_rtentry_to_entry(); 
extern int  rt6_add_entry(); 
extern int  rt6_change_default(); 
extern int  rt6_delete_default(); 
extern int  rt6_delete_entry(); 
extern int  rt6_find_entry(); 
extern int  rt6_get_default(); 
extern int  rt6_netif_add_hook(); 
extern int  rt6_netif_linkstat_hook(); 
extern int  rt6_netif_remove_hook(); 
extern int  rt6_netmask_from_prefix(); 
extern int  rt6_netmask_to_prefix(); 
extern int  rt6_route_gateway_hook(); 
extern int  rt6_route_search_hook(); 
extern int  rt6_total_entry(); 
extern int  rt6_traversal_entry(); 
extern int  rt6_entry_to_msghdr(); 
extern int  rt6_entry_to_rtentry(); 
extern int  rt6_msghdr_to_entry(); 
extern int  rt6_rtentry_to_entry(); 
extern int  tcp_forward_mss_adj(); 
extern int  netconn_accept(); 
extern int  netconn_bind(); 
extern int  netconn_bind_if(); 
extern int  netconn_close(); 
extern int  netconn_connect(); 
extern int  netconn_delete(); 
extern int  netconn_disconnect(); 
extern int  netconn_err(); 
extern int  netconn_getaddr(); 
extern int  netconn_gethostbyname_addrtype(); 
extern int  netconn_join_leave_group(); 
extern int  netconn_join_leave_group_netif(); 
extern int  netconn_listen_with_backlog(); 
extern int  netconn_new_with_proto_and_callback(); 
extern int  netconn_prepare_delete(); 
extern int  netconn_recv(); 
extern int  netconn_recv_tcp_pbuf(); 
extern int  netconn_recv_tcp_pbuf_flags(); 
extern int  netconn_recv_udp_raw_netbuf(); 
extern int  netconn_recv_udp_raw_netbuf_flags(); 
extern int  netconn_send(); 
extern int  netconn_sendto(); 
extern int  netconn_shutdown(); 
extern int  netconn_tcp_recvd(); 
extern int  netconn_thread_cleanup(); 
extern int  netconn_thread_init(); 
extern int  netconn_write_partly(); 
extern int  netconn_write_vectors_partly(); 
extern int  lwip_netconn_do_accepted(); 
extern int  lwip_netconn_do_bind(); 
extern int  lwip_netconn_do_bind_if(); 
extern int  lwip_netconn_do_close(); 
extern int  lwip_netconn_do_connect(); 
extern int  lwip_netconn_do_delconn(); 
extern int  lwip_netconn_do_disconnect(); 
extern int  lwip_netconn_do_getaddr(); 
extern int  lwip_netconn_do_gethostbyname(); 
extern int  lwip_netconn_do_join_leave_group(); 
extern int  lwip_netconn_do_join_leave_group_netif(); 
extern int  lwip_netconn_do_listen(); 
extern int  lwip_netconn_do_newconn(); 
extern int  lwip_netconn_do_recv(); 
extern int  lwip_netconn_do_send(); 
extern int  lwip_netconn_do_write(); 
extern int  lwip_netconn_is_deallocated_msg(); 
extern int  lwip_netconn_is_err_msg(); 
extern int  netconn_alloc(); 
extern int  netconn_free(); 
extern int  err_to_errno(); 
extern int  netbuf_alloc(); 
extern int  netbuf_chain(); 
extern int  netbuf_data(); 
extern int  netbuf_delete(); 
extern int  netbuf_first(); 
extern int  netbuf_free(); 
extern int  netbuf_new(); 
extern int  netbuf_next(); 
extern int  netbuf_ref(); 
extern int  lwip_freeaddrinfo(); 
extern int  lwip_getaddrinfo(); 
extern int  lwip_gethostbyname(); 
extern int  lwip_gethostbyname_r(); 
extern int  netifapi_arp_add(); 
extern int  netifapi_arp_cleanup(); 
extern int  netifapi_arp_remove(); 
extern int  netifapi_arp_traversal(); 
extern int  netifapi_arp_update(); 
extern int  netifapi_nd6_cleanup(); 
extern int  netifapi_nd6_traversal(); 
extern int  netifapi_netif_add(); 
extern int  netifapi_netif_common(); 
extern int  netifapi_netif_index_to_name(); 
extern int  netifapi_netif_name_to_index(); 
extern int  netifapi_netif_set_addr(); 
extern int  __lwip_have_event(); 
extern int  __lwip_set_sockfile(); 
extern int  lwip_accept(); 
extern int  lwip_bind(); 
extern int  lwip_close(); 
extern int  lwip_connect(); 
extern int  lwip_fcntl(); 
extern int  lwip_getpeername(); 
extern int  lwip_getsockname(); 
extern int  lwip_getsockopt(); 
extern int  lwip_inet_ntop(); 
extern int  lwip_inet_pton(); 
extern int  lwip_ioctl(); 
extern int  lwip_listen(); 
extern int  lwip_read(); 
extern int  lwip_readv(); 
extern int  lwip_recv(); 
extern int  lwip_recvfrom(); 
extern int  lwip_recvmsg(); 
extern int  lwip_send(); 
extern int  lwip_sendmsg(); 
extern int  lwip_sendto(); 
extern int  lwip_setsockopt(); 
extern int  lwip_shutdown(); 
extern int  lwip_socket(); 
extern int  lwip_socket_dbg_get_socket(); 
extern int  lwip_socket_thread_cleanup(); 
extern int  lwip_socket_thread_init(); 
extern int  lwip_write(); 
extern int  lwip_writev(); 
extern int  mem_free_callback(); 
extern int  pbuf_free_callback(); 
extern int  tcpip_api_call(); 
extern int  tcpip_callback(); 
extern int  tcpip_callbackmsg_delete(); 
extern int  tcpip_callbackmsg_new(); 
extern int  tcpip_callbackmsg_trycallback(); 
extern int  tcpip_callbackmsg_trycallback_fromisr(); 
extern int  tcpip_init(); 
extern int  tcpip_inpkt(); 
extern int  tcpip_inpkt_lost(); 
extern int  tcpip_input(); 
extern int  tcpip_qos_dontdrop_stat(); 
extern int  tcpip_qos_get(); 
extern int  tcpip_qos_set(); 
extern int  tcpip_qos_stat(); 
extern int  tcpip_send_msg_wait_sem(); 
extern int  tcpip_try_callback(); 
extern int  lwip_htonl(); 
extern int  lwip_htons(); 
extern int  lwip_itoa(); 
extern int  lwip_stricmp(); 
extern int  lwip_strnicmp(); 
extern int  lwip_strnistr(); 
extern int  lwip_strnstr(); 
extern int  dns_gethostbyname(); 
extern int  dns_gethostbyname_addrtype(); 
extern int  dns_getserver(); 
extern int  dns_init(); 
extern int  dns_local_addhost(); 
extern int  dns_local_iterate(); 
extern int  dns_local_lookup(); 
extern int  dns_local_removehost(); 
extern int  dns_setserver(); 
extern int  dns_tmr(); 
extern int  inet_chksum(); 
extern int  inet_chksum_adjust(); 
extern int  inet_chksum_pbuf(); 
extern int  inet_chksum_pseudo(); 
extern int  inet_chksum_pseudo_partial(); 
extern int  ip_chksum_pseudo(); 
extern int  ip_chksum_pseudo_partial(); 
extern int  ip6_chksum_pseudo(); 
extern int  ip6_chksum_pseudo_partial(); 
extern int  lwip_standard_chksum(); 
extern int  lwip_init(); 
extern int  ip_input(); 
extern int  ipaddr_aton(); 
extern int  ipaddr_ntoa(); 
extern int  ipaddr_ntoa_r(); 
extern int  mcast_block_group(); 
extern int  mcast_block_netif(); 
extern int  mcast_get_groupfilter_netif(); 
extern int  mcast_get_msfilter_netif(); 
extern int  mcast_input_local_match(); 
extern int  mcast_ip4_filter_info(); 
extern int  mcast_ip4_filter_interest(); 
extern int  mcast_ip4_filter_trigger_callback(); 
extern int  mcast_ip4_mc_find(); 
extern int  mcast_ip6_filter_info(); 
extern int  mcast_ip6_filter_interest(); 
extern int  mcast_ip6_filter_trigger_callback(); 
extern int  mcast_join_group(); 
extern int  mcast_join_netif(); 
extern int  mcast_leave_group(); 
extern int  mcast_leave_netif(); 
extern int  mcast_pcb_remove(); 
extern int  mcast_set_groupfilter_netif(); 
extern int  mcast_set_msfilter_netif(); 
extern int  mcast_sock_add_drop_membership(); 
extern int  mcast_sock_add_drop_source_membership(); 
extern int  mcast_sock_block_unblock_source(); 
extern int  mcast_sock_block_unblock_source_group(); 
extern int  mcast_sock_get_groupfilter(); 
extern int  mcast_sock_get_msfilter(); 
extern int  mcast_sock_ipv6_add_drop_membership(); 
extern int  mcast_sock_join_leave_group(); 
extern int  mcast_sock_join_leave_source_group(); 
extern int  mcast_sock_set_groupfilter(); 
extern int  mcast_sock_set_msfilter(); 
extern int  mcast_unblock_group(); 
extern int  mcast_unblock_netif(); 
extern int  mem_calloc(); 
extern int  mem_free(); 
extern int  mem_init(); 
extern int  mem_malloc(); 
extern int  mem_trim(); 
extern int  memp_free(); 
extern int  memp_free_pool(); 
extern int  memp_init(); 
extern int  memp_init_pool(); 
extern int  memp_malloc(); 
extern int  memp_malloc_pool(); 
extern int  netif_add(); 
extern int  netif_add_ext_callback(); 
extern int  netif_add_ip6_address(); 
extern int  netif_add_noaddr(); 
extern int  netif_create_ip6_linklocal_address(); 
extern int  netif_find(); 
extern int  netif_get_by_index(); 
extern int  netif_get_ip6_addr_match(); 
extern int  netif_get_name(); 
extern int  netif_get_total(); 
extern int  netif_index_to_name(); 
extern int  netif_init(); 
extern int  netif_input(); 
extern int  netif_invoke_ext_callback(); 
extern int  netif_ip4_addr_cmp(); 
extern int  netif_ip6_addr_set(); 
extern int  netif_ip6_addr_set_parts(); 
extern int  netif_ip6_addr_set_state(); 
extern int  netif_ip6_gw_set(); 
extern int  netif_loop_output(); 
extern int  netif_name_to_index(); 
extern int  netif_poll(); 
extern int  netif_remove(); 
extern int  netif_remove_ext_callback(); 
extern int  netif_set_addr(); 
extern int  netif_set_default(); 
extern int  netif_set_down(); 
extern int  netif_set_gw(); 
extern int  netif_set_ipaddr(); 
extern int  netif_set_link_callback(); 
extern int  netif_set_link_down(); 
extern int  netif_set_link_up(); 
extern int  netif_set_netmask(); 
extern int  netif_set_remove_callback(); 
extern int  netif_set_status_callback(); 
extern int  netif_set_up(); 
extern int  pbuf_add_header(); 
extern int  pbuf_add_header_force(); 
extern int  pbuf_alloc(); 
extern int  pbuf_alloc_reference(); 
extern int  pbuf_alloced_custom(); 
extern int  pbuf_cat(); 
extern int  pbuf_chain(); 
extern int  pbuf_clen(); 
extern int  pbuf_clone(); 
extern int  pbuf_coalesce(); 
extern int  pbuf_copy(); 
extern int  pbuf_copy_partial(); 
extern int  pbuf_copy_partial_pbuf(); 
extern int  pbuf_dechain(); 
extern int  pbuf_free(); 
extern int  pbuf_free_header(); 
extern int  pbuf_get_at(); 
extern int  pbuf_get_contiguous(); 
extern int  pbuf_header(); 
extern int  pbuf_header_force(); 
extern int  pbuf_memcmp(); 
extern int  pbuf_memfind(); 
extern int  pbuf_put_at(); 
extern int  pbuf_realloc(); 
extern int  pbuf_ref(); 
extern int  pbuf_remove_header(); 
extern int  pbuf_skip(); 
extern int  pbuf_split_64k(); 
extern int  pbuf_strstr(); 
extern int  pbuf_take(); 
extern int  pbuf_take_at(); 
extern int  pbuf_try_get_at(); 
extern int  raw_bind(); 
extern int  raw_bind_netif(); 
extern int  raw_connect(); 
extern int  raw_disconnect(); 
extern int  raw_input(); 
extern int  raw_netif_ip_addr_changed(); 
extern int  raw_new(); 
extern int  raw_new_ip_type(); 
extern int  raw_recv(); 
extern int  raw_remove(); 
extern int  raw_send(); 
extern int  raw_sendto(); 
extern int  raw_sendto_if_src(); 
extern int  stats_display(); 
extern int  stats_display_igmp(); 
extern int  stats_display_mem(); 
extern int  stats_display_memp(); 
extern int  stats_display_proto(); 
extern int  stats_display_sys(); 
extern int  stats_init(); 
extern int  tcp_abandon(); 
extern int  tcp_abort(); 
extern int  tcp_accept(); 
extern int  tcp_alloc(); 
extern int  tcp_arg(); 
extern int  tcp_backlog_accepted(); 
extern int  tcp_backlog_delayed(); 
extern int  tcp_bind(); 
extern int  tcp_bind_netif(); 
extern int  tcp_close(); 
extern int  tcp_connect(); 
extern int  tcp_debug_state_str(); 
extern int  tcp_eff_send_mss_netif(); 
extern int  tcp_err(); 
extern int  tcp_fasttmr(); 
extern int  tcp_free(); 
extern int  tcp_free_ooseq(); 
extern int  tcp_init(); 
extern int  tcp_listen_with_backlog(); 
extern int  tcp_listen_with_backlog_and_err(); 
extern int  tcp_netif_ip_addr_changed(); 
extern int  tcp_new(); 
extern int  tcp_new_ip_type(); 
extern int  tcp_next_iss(); 
extern int  tcp_pcb_purge(); 
extern int  tcp_pcb_remove(); 
extern int  tcp_poll(); 
extern int  tcp_process_refused_data(); 
extern int  tcp_recv(); 
extern int  tcp_recv_null(); 
extern int  tcp_recved(); 
extern int  tcp_seg_copy(); 
extern int  tcp_seg_free(); 
extern int  tcp_segs_free(); 
extern int  tcp_sent(); 
extern int  tcp_setprio(); 
extern int  tcp_shutdown(); 
extern int  tcp_slowtmr(); 
extern int  tcp_tcp_get_tcp_addrinfo(); 
extern int  tcp_tmr(); 
extern int  tcp_txnow(); 
extern int  tcp_update_rcv_ann_wnd(); 
extern int  tcp_input(); 
extern int  tcp_trigger_input_pcb_close(); 
extern int  tcp_enqueue_flags(); 
extern int  tcp_keepalive(); 
extern int  tcp_output(); 
extern int  tcp_rexmit(); 
extern int  tcp_rexmit_fast(); 
extern int  tcp_rexmit_rto(); 
extern int  tcp_rexmit_rto_commit(); 
extern int  tcp_rexmit_rto_prepare(); 
extern int  tcp_rst(); 
extern int  tcp_send_empty_ack(); 
extern int  tcp_send_fin(); 
extern int  tcp_split_unsent_seg(); 
extern int  tcp_write(); 
extern int  tcp_zero_window_probe(); 
extern int  sys_check_timeouts(); 
extern int  sys_restart_timeouts(); 
extern int  sys_timeout(); 
extern int  sys_timeouts_init(); 
extern int  sys_timeouts_sleeptime(); 
extern int  sys_untimeout(); 
extern int  tcp_timer_needed(); 
extern int  udp_bind(); 
extern int  udp_bind_netif(); 
extern int  udp_connect(); 
extern int  udp_disconnect(); 
extern int  udp_init(); 
extern int  udp_input(); 
extern int  udp_netif_ip_addr_changed(); 
extern int  udp_new(); 
extern int  udp_new_ip_type(); 
extern int  udp_recv(); 
extern int  udp_remove(); 
extern int  udp_send(); 
extern int  udp_sendto(); 
extern int  udp_sendto_if(); 
extern int  udp_sendto_if_src(); 
extern int  autoip_accept_packet(); 
extern int  autoip_arp_reply(); 
extern int  autoip_network_changed(); 
extern int  autoip_set_struct(); 
extern int  autoip_start(); 
extern int  autoip_stop(); 
extern int  autoip_supplied_address(); 
extern int  autoip_tmr(); 
extern int  dhcp_arp_reply(); 
extern int  dhcp_cleanup(); 
extern int  dhcp_coarse_tmr(); 
extern int  dhcp_fine_tmr(); 
extern int  dhcp_inform(); 
extern int  dhcp_network_changed(); 
extern int  dhcp_release(); 
extern int  dhcp_release_and_stop(); 
extern int  dhcp_renew(); 
extern int  dhcp_set_struct(); 
extern int  dhcp_start(); 
extern int  dhcp_stop(); 
extern int  dhcp_supplied_address(); 
extern int  etharp_add_static_entry(); 
extern int  etharp_cleanup_netif(); 
extern int  etharp_find_addr(); 
extern int  etharp_get_entry(); 
extern int  etharp_input(); 
extern int  etharp_output(); 
extern int  etharp_query(); 
extern int  etharp_raw(); 
extern int  etharp_remove_static_entry(); 
extern int  etharp_request(); 
extern int  etharp_tmr(); 
extern int  etharp_traversal(); 
extern int  etharp_update_arp_entry(); 
extern int  icmp_dest_unreach(); 
extern int  icmp_input(); 
extern int  icmp_time_exceeded(); 
extern int  igmp_init(); 
extern int  igmp_input(); 
extern int  igmp_joingroup(); 
extern int  igmp_joingroup_netif(); 
extern int  igmp_leavegroup(); 
extern int  igmp_leavegroup_netif(); 
extern int  igmp_lookfor_group(); 
extern int  igmp_report_groups(); 
extern int  igmp_start(); 
extern int  igmp_stop(); 
extern int  igmp_tmr(); 
extern int  ip4_forward_get(); 
extern int  ip4_forward_set(); 
extern int  ip4_get_ip_id(); 
extern int  ip4_input(); 
extern int  ip4_output(); 
extern int  ip4_output_hinted(); 
extern int  ip4_output_if(); 
extern int  ip4_output_if_opt(); 
extern int  ip4_output_if_opt_src(); 
extern int  ip4_output_if_src(); 
extern int  ip4_route(); 
extern int  ip4_route_src(); 
extern int  ip4_set_default_multicast_netif(); 
extern int  ip4_addr_isbroadcast_u32(); 
extern int  ip4_addr_netmask_valid(); 
extern int  ip4addr_aton(); 
extern int  ip4addr_ntoa(); 
extern int  ip4addr_ntoa_r(); 
extern int  ipaddr_addr(); 
extern int  ip_reass_tmr(); 
extern int  ip4_frag(); 
extern int  ip4_reass(); 
extern int  dhcp6_cleanup(); 
extern int  dhcp6_disable(); 
extern int  dhcp6_enable_stateful(); 
extern int  dhcp6_enable_stateless(); 
extern int  dhcp6_nd6_ra_trigger(); 
extern int  dhcp6_set_struct(); 
extern int  dhcp6_tmr(); 
extern int  ethip6_output(); 
extern int  icmp6_dest_unreach(); 
extern int  icmp6_input(); 
extern int  icmp6_packet_too_big(); 
extern int  icmp6_param_problem(); 
extern int  icmp6_time_exceeded(); 
extern int  icmp6_time_exceeded_with_addrs(); 
extern int  ip6_forward_get(); 
extern int  ip6_forward_set(); 
extern int  ip6_input(); 
extern int  ip6_options_add_hbh_ra(); 
extern int  ip6_output(); 
extern int  ip6_output_hinted(); 
extern int  ip6_output_if(); 
extern int  ip6_output_if_src(); 
extern int  ip6_route(); 
extern int  ip6_select_source_address(); 
extern int  ip6addr_aton(); 
extern int  ip6addr_ntoa(); 
extern int  ip6addr_ntoa_r(); 
extern int  ip6_frag(); 
extern int  ip6_reass(); 
extern int  ip6_reass_tmr(); 
extern int  mld6_input(); 
extern int  mld6_joingroup(); 
extern int  mld6_joingroup_netif(); 
extern int  mld6_leavegroup(); 
extern int  mld6_leavegroup_netif(); 
extern int  mld6_lookfor_group(); 
extern int  mld6_report_groups(); 
extern int  mld6_stop(); 
extern int  mld6_tmr(); 
extern int  nd6_adjust_mld_membership(); 
extern int  nd6_cleanup_netif(); 
extern int  nd6_clear_destination_cache(); 
extern int  nd6_find_route(); 
extern int  nd6_get_destination_mtu(); 
extern int  nd6_get_next_hop_addr_or_queue(); 
extern int  nd6_input(); 
extern int  nd6_reachability_hint(); 
extern int  nd6_restart_netif(); 
extern int  nd6_tmr(); 
extern int  nd6_traversal(); 
extern int  ethernet_input(); 
extern int  ethernet_output(); 
extern int  pktq_get(); 
extern int  pktq_init(); 
extern int  pktq_isempty(); 
extern int  pktq_put(); 
extern int  ble_addr_to_eui64(); 
extern int  eui64_to_ble_addr(); 
extern int  rfc7668_get_context(); 
extern int  rfc7668_if_init(); 
extern int  rfc7668_input(); 
extern int  rfc7668_output(); 
extern int  rfc7668_set_context(); 
extern int  rfc7668_set_local_addr_eui64(); 
extern int  rfc7668_set_local_addr_mac48(); 
extern int  rfc7668_set_peer_addr_eui64(); 
extern int  rfc7668_set_peer_addr_mac48(); 
extern int  tcpip_rfc7668_input(); 
extern int  lowpan6_compress_headers(); 
extern int  lowpan6_decompress(); 
extern int  lowpan6_get_address_mode(); 
extern int  lowpan6_calc_crc(); 
extern int  lowpan6_get_context(); 
extern int  lowpan6_get_pan_id(); 
extern int  lowpan6_get_short_addr(); 
extern int  lowpan6_if_init(); 
extern int  lowpan6_input(); 
extern int  lowpan6_output(); 
extern int  lowpan6_set_context(); 
extern int  lowpan6_set_pan_id(); 
extern int  lowpan6_set_short_addr(); 
extern int  lowpan6_tmr(); 
extern int  tcpip_6lowpan_input(); 
extern int  slipif_init(); 
extern int  slipif_poll(); 
extern int  zepif_init(); 
extern int  aodv_hello_process(); 
extern int  aodv_hello_reset(); 
extern int  aodv_hello_send(); 
extern int  aodv_hello_start(); 
extern int  aodv_hello_stop(); 
extern int  aodv_hello_update_timeout(); 
extern int  aodv_expanding_ring_search_get(); 
extern int  aodv_expanding_ring_search_set(); 
extern int  aodv_gw_get(); 
extern int  aodv_gw_set(); 
extern int  aodv_netif_add(); 
extern int  aodv_netif_output(); 
extern int  aodv_netif_output6(); 
extern int  aodv_netif_remove(); 
extern int  aodv_wait_on_reboot_get(); 
extern int  aodv_neighbor_add(); 
extern int  aodv_neighbor_link_break(); 
extern int  aodv_udp_new(); 
extern int  aodv_udp_remove(); 
extern int  aodv_udp_sendto(); 
extern int  aodv_rerr_add_udest(); 
extern int  aodv_rerr_create(); 
extern int  aodv_rerr_process(); 
extern int  aodv_precursor_add(); 
extern int  aodv_precursor_list_destroy(); 
extern int  aodv_precursor_remove(); 
extern int  aodv_rt_delete(); 
extern int  aodv_rt_delete_netif(); 
extern int  aodv_rt_find(); 
extern int  aodv_rt_find_gateway(); 
extern int  aodv_rt_init(); 
extern int  aodv_rt_insert(); 
extern int  aodv_rt_invalidate(); 
extern int  aodv_rt_netif_index_get(); 
extern int  aodv_rt_traversal(); 
extern int  aodv_rt_update(); 
extern int  aodv_rt_update_inet_rt(); 
extern int  aodv_rt_update_route_timeouts(); 
extern int  aodv_rt_update_timeout(); 
extern int  aodv_rrep_ack_create(); 
extern int  aodv_rrep_ack_process(); 
extern int  aodv_rrep_create(); 
extern int  aodv_rrep_ext_add(); 
extern int  aodv_rrep_forward(); 
extern int  aodv_rrep_process(); 
extern int  aodv_rrep_send(); 
extern int  aodv_rreq_blacklist_find(); 
extern int  aodv_rreq_blacklist_insert(); 
extern int  aodv_rreq_blacklist_timeout(); 
extern int  aodv_rreq_create(); 
extern int  aodv_rreq_forward(); 
extern int  aodv_rreq_local_repair(); 
extern int  aodv_rreq_process(); 
extern int  aodv_rreq_record_find(); 
extern int  aodv_rreq_record_insert(); 
extern int  aodv_rreq_record_timeout(); 
extern int  aodv_rreq_route_discovery(); 
extern int  aodv_rreq_send(); 
extern int  aodv_rreq_send_ext(); 
extern int  aodv_seeklist_find(); 
extern int  aodv_seeklist_insert(); 
extern int  aodv_seeklist_remove(); 
extern int  aodv_timer_add(); 
extern int  aodv_timer_init(); 
extern int  aodv_timer_init_svr(); 
extern int  aodv_timer_left(); 
extern int  aodv_timer_remove(); 
extern int  aodv_timer_set_timeout(); 
extern int  aodv_timer_timeout_now(); 
extern int  aodv_hello_timeout(); 
extern int  aodv_local_repair_timeout(); 
extern int  aodv_route_delete_timeout(); 
extern int  aodv_route_discovery_timeout(); 
extern int  aodv_route_expire_timeout(); 
extern int  aodv_rrep_ack_timeout(); 
extern int  aodv_wait_on_reboot_timeout(); 
extern int  auth_withpeer_fail(); 
extern int  auth_withpeer_success(); 
extern int  continue_networks(); 
extern int  get_secret(); 
extern int  link_down(); 
extern int  link_established(); 
extern int  link_required(); 
extern int  link_terminated(); 
extern int  np_down(); 
extern int  np_finished(); 
extern int  np_up(); 
extern int  start_networks(); 
extern int  upper_layers_down(); 
extern int  chap_auth_with_peer(); 
extern int  eap_authwithpeer(); 
extern int  eui64_ntoa(); 
extern int  fsm_close(); 
extern int  fsm_init(); 
extern int  fsm_input(); 
extern int  fsm_lowerdown(); 
extern int  fsm_lowerup(); 
extern int  fsm_open(); 
extern int  fsm_protreject(); 
extern int  fsm_sdata(); 
extern int  lcp_close(); 
extern int  lcp_lowerdown(); 
extern int  lcp_lowerup(); 
extern int  lcp_open(); 
extern int  lcp_sprotrej(); 
extern int  magic(); 
extern int  magic_init(); 
extern int  magic_pow(); 
extern int  magic_random_bytes(); 
extern int  magic_randomize(); 
extern int  cdns(); 
extern int  cif6addr(); 
extern int  cifaddr(); 
extern int  get_mask(); 
extern int  netif_get_mtu(); 
extern int  netif_set_mtu(); 
extern int  new_phase(); 
extern int  ppp_close(); 
extern int  ppp_connect(); 
extern int  ppp_free(); 
extern int  ppp_init(); 
extern int  ppp_input(); 
extern int  ppp_ioctl(); 
extern int  ppp_link_end(); 
extern int  ppp_link_failed(); 
extern int  ppp_link_terminated(); 
extern int  ppp_new(); 
extern int  ppp_recv_config(); 
extern int  ppp_send_config(); 
extern int  ppp_set_auth(); 
extern int  ppp_set_notify_phase_callback(); 
extern int  ppp_start(); 
extern int  ppp_write(); 
extern int  sdns(); 
extern int  sif6addr(); 
extern int  sif6down(); 
extern int  sif6up(); 
extern int  sifaddr(); 
extern int  sifdown(); 
extern int  sifup(); 
extern int  sifvjcomp(); 
extern int  pppapi_close(); 
extern int  pppapi_connect(); 
extern int  pppapi_free(); 
extern int  pppapi_ioctl(); 
extern int  pppapi_pppoe_create(); 
extern int  pppapi_pppol2tp_create(); 
extern int  pppapi_pppos_create(); 
extern int  pppapi_set_default(); 
extern int  pppapi_set_notify_phase_callback(); 
extern int  pppcrypt_56_to_64_bit_key(); 
extern int  pppoe_create(); 
extern int  pppoe_data_input(); 
extern int  pppoe_disc_input(); 
extern int  pppol2tp_create(); 
extern int  pppos_create(); 
extern int  pppos_input(); 
extern int  upap_authwithpeer(); 
extern int  ppp_dbglog_impl(); 
extern int  ppp_error_impl(); 
extern int  ppp_fatal_impl(); 
extern int  ppp_info_impl(); 
extern int  ppp_notice_impl(); 
extern int  ppp_print_string(); 
extern int  ppp_slprintf(); 
extern int  ppp_strlcat(); 
extern int  ppp_strlcpy(); 
extern int  ppp_vslprintf(); 
extern int  ppp_warn_impl(); 
extern int  vj_compress_init(); 
extern int  vj_compress_tcp(); 
extern int  vj_uncompress_err(); 
extern int  vj_uncompress_tcp(); 
extern int  vj_uncompress_uncomp(); 
extern int  crypt_decode(); 
extern int  crypt_encode(); 
extern int  crypt_init(); 
extern int  crypt_verify(); 
extern int  ieee802154_from_eth(); 
extern int  ieee802154_hdr_len(); 
extern int  ieee802154_seqno_set(); 
extern int  ieee802154_to_eth(); 
extern int  ieee802154_frame_create_ack(); 
extern int  ieee802154_frame_create_hdr(); 
extern int  ieee802154_frame_hdrlen(); 
extern int  ieee802154_frame_parse(); 
extern int  lowpan_context_addr_add(); 
extern int  lowpan_context_addr_delete(); 
extern int  lowpan_header_create(); 
extern int  lowpan_header_prase(); 
extern int  lowpan_frag(); 
extern int  lowpan_reass(); 
extern int  lowpan_reass_init(); 
extern int  lowpan_reass_tmr(); 
extern int  lowpan_input(); 
extern int  lowpan_netif_add(); 
extern int  lowpan_netif_autoip_start(); 
extern int  lowpan_netif_dhcp_start(); 
extern int  lowpan_netif_set_default(); 
extern int  lowpan_netif_set_up(); 
extern int  mac_input_callback(); 
extern int  mac_send_callback(); 
extern int  API_INetFtpServerBindDev(); 
extern int  API_INetFtpServerInit(); 
extern int  API_INetFtpServerPath(); 
extern int  API_INetFtpServerShow(); 
extern int  __inetHostTableGetItem(); 
extern int  __inetHostTableInit(); 
extern int  API_INetHostTableAddItem(); 
extern int  API_INetHostTableDelItem(); 
extern int  API_INetHostTableGetItem(); 
extern int  __inetIacFilter(); 
extern int  __inetIacMakeFrame(); 
extern int  __inetIacSend(); 
extern int  API_INetNatAliasAdd(); 
extern int  API_INetNatAliasDelete(); 
extern int  API_INetNatInit(); 
extern int  API_INetNatIpFragGet(); 
extern int  API_INetNatIpFragSet(); 
extern int  API_INetNatLocalAdd(); 
extern int  API_INetNatLocalDelete(); 
extern int  API_INetNatMapAdd(); 
extern int  API_INetNatMapDelete(); 
extern int  API_INetNatStart(); 
extern int  API_INetNatStop(); 
extern int  API_INetNatWanAdd(); 
extern int  API_INetNatWanDelete(); 
extern int  __natAddAp(); 
extern int  __natAddLocal(); 
extern int  __natAliasAdd(); 
extern int  __natAliasDelete(); 
extern int  __natDeleteAp(); 
extern int  __natDeleteLocal(); 
extern int  __natInit(); 
extern int  __natIpFragGet(); 
extern int  __natIpFragSet(); 
extern int  __natMapAdd(); 
extern int  __natMapDelete(); 
extern int  __natStart(); 
extern int  __natStop(); 
extern int  __procFsNatInit(); 
extern int  nat_netif_add_hook(); 
extern int  nat_netif_remove_hook(); 
extern int  API_INetNetBiosBindDev(); 
extern int  API_INetNetBiosInit(); 
extern int  API_INetNetBiosNameGet(); 
extern int  API_INetNetBiosNameSet(); 
extern int  netbiosns_binddev(); 
extern int  netbiosns_init(); 
extern int  netbiosns_stop(); 
extern int  API_INetNpfAllowGet(); 
extern int  API_INetNpfDropGet(); 
extern int  API_INetNpfInit(); 
extern int  API_INetNpfRuleAdd(); 
extern int  API_INetNpfRuleAddEx(); 
extern int  API_INetNpfRuleDel(); 
extern int  API_INetNpfRuleDelEx(); 
extern int  API_INetNpfShow(); 
extern int  npf_netif_attach(); 
extern int  npf_netif_detach(); 
extern int  API_INetPing(); 
extern int  API_INetPingInit(); 
extern int  API_INetPing6(); 
extern int  API_INetPing6Init(); 
extern int  API_PppConnect(); 
extern int  API_PppDelete(); 
extern int  API_PppDisconnect(); 
extern int  API_PppGetPhase(); 
extern int  API_PppOeCreate(); 
extern int  API_PppOl2tpCreate(); 
extern int  API_PppOsCreate(); 
extern int  API_INetQosInit(); 
extern int  API_INetQosRuleAdd(); 
extern int  API_INetQosRuleDel(); 
extern int  API_INetQosShow(); 
extern int  qos_netif_attach(); 
extern int  qos_netif_detach(); 
extern int  xdr_authdes_cred(); 
extern int  xdr_authdes_verf(); 
extern int  xdr_authunix_parms(); 
extern int  authnone_create(); 
extern int  authunix_create(); 
extern int  authunix_create_default(); 
extern int  bindresvport(); 
extern int  clnt_create(); 
extern int  clnt_pcreateerror(); 
extern int  clnt_perrno(); 
extern int  clnt_perror(); 
extern int  clnt_spcreateerror(); 
extern int  clnt_sperrno(); 
extern int  clnt_sperror(); 
extern int  libc_freeres_fn(); 
extern int  clntraw_create(); 
extern int  callrpc(); 
extern int  clnttcp_create(); 
extern int  __libc_clntudp_bufcreate(); 
extern int  clntudp_bufcreate(); 
extern int  clntudp_create(); 
extern int  clntunix_create(); 
extern int  _create_xid(); 
extern int  cbc_crypt(); 
extern int  ecb_crypt(); 
extern int  _des_crypt(); 
extern int  des_setparity(); 
extern int  __getprotobyname_r(); 
extern int  getrpcport(); 
extern int  get_myaddress(); 
extern int  key_decryptsession(); 
extern int  key_decryptsession_pk(); 
extern int  key_encryptsession(); 
extern int  key_encryptsession_pk(); 
extern int  key_gendes(); 
extern int  key_get_conv(); 
extern int  key_secretkey_is_set(); 
extern int  key_setnet(); 
extern int  key_setsecret(); 
extern int  xdr_cryptkeyarg(); 
extern int  xdr_cryptkeyarg2(); 
extern int  xdr_cryptkeyres(); 
extern int  xdr_getcredres(); 
extern int  xdr_key_netstarg(); 
extern int  xdr_key_netstres(); 
extern int  xdr_keybuf(); 
extern int  xdr_keystatus(); 
extern int  xdr_netnamestr(); 
extern int  xdr_unixcred(); 
extern int  pmap_set(); 
extern int  pmap_unset(); 
extern int  xdr_pmap(); 
extern int  xdr_pmaplist(); 
extern int  clnt_broadcast(); 
extern int  pmap_rmtcall(); 
extern int  xdr_rmtcall_args(); 
extern int  xdr_rmtcallres(); 
extern int  pmap_getmaps(); 
extern int  __get_socket(); 
extern int  __libc_rpc_getport(); 
extern int  pmap_getport(); 
extern int  xdr_callmsg(); 
extern int  _rpc_dtablesize(); 
extern int  _seterr_reply(); 
extern int  xdr_accepted_reply(); 
extern int  xdr_callhdr(); 
extern int  xdr_des_block(); 
extern int  xdr_opaque_auth(); 
extern int  xdr_rejected_reply(); 
extern int  xdr_replymsg(); 
extern int  rtime(); 
extern int  __svc_accept_failed(); 
extern int  svc_getreq(); 
extern int  svc_getreq_common(); 
extern int  svc_getreq_poll(); 
extern int  svc_getreqset(); 
extern int  svc_register(); 
extern int  svc_sendreply(); 
extern int  svc_unregister(); 
extern int  svcerr_auth(); 
extern int  svcerr_decode(); 
extern int  svcerr_noproc(); 
extern int  svcerr_noprog(); 
extern int  svcerr_progvers(); 
extern int  svcerr_systemerr(); 
extern int  svcerr_weakauth(); 
extern int  xprt_register(); 
extern int  xprt_unregister(); 
extern int  _authenticate(); 
extern int  _svcauth_short(); 
extern int  _svcauth_unix(); 
extern int  svcraw_create(); 
extern int  svc_exit(); 
extern int  svc_run(); 
extern int  __registerrpc(); 
extern int  svcfd_create(); 
extern int  svctcp_create(); 
extern int  svcudp_bufcreate(); 
extern int  svcudp_create(); 
extern int  svcudp_enablecache(); 
extern int  svcunix_create(); 
extern int  svcunixfd_create(); 
extern int  passwd2des(); 
extern int  passwd2des_internal(); 
extern int  xdecrypt(); 
extern int  xencrypt(); 
extern int  xdr_bool(); 
extern int  xdr_bytes(); 
extern int  xdr_char(); 
extern int  xdr_enum(); 
extern int  xdr_free(); 
extern int  xdr_hyper(); 
extern int  xdr_int(); 
extern int  xdr_long(); 
extern int  xdr_longlong_t(); 
extern int  xdr_netobj(); 
extern int  xdr_opaque(); 
extern int  xdr_short(); 
extern int  xdr_string(); 
extern int  xdr_u_char(); 
extern int  xdr_u_hyper(); 
extern int  xdr_u_int(); 
extern int  xdr_u_long(); 
extern int  xdr_u_longlong_t(); 
extern int  xdr_u_short(); 
extern int  xdr_union(); 
extern int  xdr_void(); 
extern int  xdr_wrapstring(); 
extern int  xdr_array(); 
extern int  xdr_vector(); 
extern int  xdr_double(); 
extern int  xdr_float(); 
extern int  xdr_int16_t(); 
extern int  xdr_int32_t(); 
extern int  xdr_int64_t(); 
extern int  xdr_int8_t(); 
extern int  xdr_quad_t(); 
extern int  xdr_u_quad_t(); 
extern int  xdr_uint16_t(); 
extern int  xdr_uint32_t(); 
extern int  xdr_uint64_t(); 
extern int  xdr_uint8_t(); 
extern int  xdrmem_create(); 
extern int  xdrrec_create(); 
extern int  xdrrec_endofrecord(); 
extern int  xdrrec_eof(); 
extern int  xdrrec_skiprecord(); 
extern int  xdr_pointer(); 
extern int  xdr_reference(); 
extern int  xdr_sizeof(); 
extern int  xdrstdio_create(); 
extern int  snmpv3_build_priv_param(); 
extern int  snmpv3_engine_id_changed(); 
extern int  snmpv3_get_engine_boots_internal(); 
extern int  snmpv3_get_engine_time_internal(); 
extern int  snmpv3_auth(); 
extern int  snmpv3_crypt(); 
extern int  snmpv3_password_to_key_md5(); 
extern int  snmpv3_password_to_key_sha(); 
extern int  parse_user_config(); 
extern int  snmpv3_get_amount_of_users(); 
extern int  snmpv3_get_engine_boots(); 
extern int  snmpv3_get_engine_boots_config(); 
extern int  snmpv3_get_engine_id(); 
extern int  snmpv3_get_engine_id_config(); 
extern int  snmpv3_get_engine_time(); 
extern int  snmpv3_get_user(); 
extern int  snmpv3_get_user_storagetype(); 
extern int  snmpv3_get_user_table_config(); 
extern int  snmpv3_get_username(); 
extern int  snmpv3_reset_engine_boots(); 
extern int  snmpv3_reset_engine_id_config(); 
extern int  snmpv3_reset_engine_time(); 
extern int  snmpv3_reset_user_table(); 
extern int  snmpv3_save_engine_boots_config(); 
extern int  snmpv3_save_engine_id_config(); 
extern int  snmpv3_set_engine_boots(); 
extern int  snmpv3_set_engine_id(); 
extern int  snmpv3_set_user_auth_algo(); 
extern int  snmpv3_set_user_auth_key(); 
extern int  snmpv3_set_user_priv_algo(); 
extern int  snmpv3_set_user_priv_key(); 
extern int  snmpv3_sylixos_init(); 
extern int  snmp_ans1_enc_tlv(); 
extern int  snmp_asn1_dec_oid(); 
extern int  snmp_asn1_dec_raw(); 
extern int  snmp_asn1_dec_s32t(); 
extern int  snmp_asn1_dec_tlv(); 
extern int  snmp_asn1_dec_u32t(); 
extern int  snmp_asn1_dec_u64t(); 
extern int  snmp_asn1_enc_length_cnt(); 
extern int  snmp_asn1_enc_oid(); 
extern int  snmp_asn1_enc_oid_cnt(); 
extern int  snmp_asn1_enc_raw(); 
extern int  snmp_asn1_enc_s32t(); 
extern int  snmp_asn1_enc_s32t_cnt(); 
extern int  snmp_asn1_enc_u32t(); 
extern int  snmp_asn1_enc_u32t_cnt(); 
extern int  snmp_asn1_enc_u64t(); 
extern int  snmp_asn1_enc_u64t_cnt(); 
extern int  netif_to_num(); 
extern int  snmp_decode_bits(); 
extern int  snmp_decode_truthvalue(); 
extern int  snmp_encode_bits(); 
extern int  snmp_encode_truthvalue(); 
extern int  snmp_get_device_enterprise_oid(); 
extern int  snmp_get_next_node_instance_from_oid(); 
extern int  snmp_get_node_instance_from_oid(); 
extern int  snmp_ip_port_to_oid(); 
extern int  snmp_ip_to_oid(); 
extern int  snmp_ip4_to_oid(); 
extern int  snmp_ip6_to_oid(); 
extern int  snmp_mib_tree_resolve_exact(); 
extern int  snmp_mib_tree_resolve_next(); 
extern int  snmp_next_oid_check(); 
extern int  snmp_next_oid_init(); 
extern int  snmp_next_oid_precheck(); 
extern int  snmp_oid_append(); 
extern int  snmp_oid_assign(); 
extern int  snmp_oid_combine(); 
extern int  snmp_oid_compare(); 
extern int  snmp_oid_equal(); 
extern int  snmp_oid_in_range(); 
extern int  snmp_oid_prefix(); 
extern int  snmp_oid_to_ip(); 
extern int  snmp_oid_to_ip_port(); 
extern int  snmp_oid_to_ip4(); 
extern int  snmp_oid_to_ip6(); 
extern int  snmp_set_device_enterprise_oid(); 
extern int  snmp_set_mibs(); 
extern int  snmp_set_test_ok(); 
extern int  snmp_mib2_lwip_synchronizer(); 
extern int  snmp_mib2_set_syscontact(); 
extern int  snmp_mib2_set_syscontact_readonly(); 
extern int  snmp_mib2_set_sysdescr(); 
extern int  snmp_mib2_set_syslocation(); 
extern int  snmp_mib2_set_syslocation_readonly(); 
extern int  snmp_mib2_set_sysname(); 
extern int  snmp_mib2_set_sysname_readonly(); 
extern int  snmp_append_outbound_varbind(); 
extern int  snmp_get_community(); 
extern int  snmp_get_community_trap(); 
extern int  snmp_get_community_write(); 
extern int  snmp_receive(); 
extern int  snmp_set_community(); 
extern int  snmp_set_community_trap(); 
extern int  snmp_set_community_write(); 
extern int  snmp_set_inform_callback(); 
extern int  snmp_set_write_callback(); 
extern int  snmp_v1_enable(); 
extern int  snmp_v1_enabled(); 
extern int  snmp_v2c_enable(); 
extern int  snmp_v2c_enabled(); 
extern int  snmp_v3_enable(); 
extern int  snmp_v3_enabled(); 
extern int  snmp_varbind_length(); 
extern int  snmp_vb_enumerator_get_next(); 
extern int  snmp_vb_enumerator_init(); 
extern int  snmp_bind_if(); 
extern int  snmp_get_local_ip_for_dst(); 
extern int  snmp_init(); 
extern int  snmp_sendto(); 
extern int  snmp_pbuf_stream_init(); 
extern int  snmp_pbuf_stream_read(); 
extern int  snmp_pbuf_stream_seek(); 
extern int  snmp_pbuf_stream_seek_abs(); 
extern int  snmp_pbuf_stream_write(); 
extern int  snmp_pbuf_stream_writebuf(); 
extern int  snmp_pbuf_stream_writeto(); 
extern int  snmp_scalar_array_get_instance(); 
extern int  snmp_scalar_array_get_next_instance(); 
extern int  snmp_scalar_get_instance(); 
extern int  snmp_scalar_get_next_instance(); 
extern int  snmp_table_extract_value_from_refconstptr(); 
extern int  snmp_table_extract_value_from_s32ref(); 
extern int  snmp_table_extract_value_from_u32ref(); 
extern int  snmp_table_get_instance(); 
extern int  snmp_table_get_next_instance(); 
extern int  snmp_table_simple_get_instance(); 
extern int  snmp_table_simple_get_next_instance(); 
extern int  snmp_threadsync_get_instance(); 
extern int  snmp_threadsync_get_next_instance(); 
extern int  snmp_threadsync_init(); 
extern int  snmp_authfail_trap(); 
extern int  snmp_coldstart_trap(); 
extern int  snmp_get_auth_traps_enabled(); 
extern int  snmp_get_default_trap_version(); 
extern int  snmp_send_inform(); 
extern int  snmp_send_inform_generic(); 
extern int  snmp_send_inform_specific(); 
extern int  snmp_send_trap(); 
extern int  snmp_send_trap_generic(); 
extern int  snmp_send_trap_specific(); 
extern int  snmp_set_auth_traps_enabled(); 
extern int  snmp_set_default_trap_version(); 
extern int  snmp_trap_dst_enable(); 
extern int  snmp_trap_dst_ip_set(); 
extern int  API_INetTelnetBindDev(); 
extern int  API_INetTelnetInit(); 
extern int  __netVlanInit(); 
extern int  API_VlanGet(); 
extern int  API_VlanSet(); 
extern int  API_VlanShow(); 
extern int  API_INetVpnInit(); 
extern int  __unix_have_event(); 
extern int  __unix_set_sockfile(); 
extern int  unix_accept(); 
extern int  unix_bind(); 
extern int  unix_close(); 
extern int  unix_connect(); 
extern int  unix_connect2(); 
extern int  unix_getpeername(); 
extern int  unix_getsockname(); 
extern int  unix_getsockopt(); 
extern int  unix_init(); 
extern int  unix_ioctl(); 
extern int  unix_listen(); 
extern int  unix_recv(); 
extern int  unix_recvfrom(); 
extern int  unix_recvmsg(); 
extern int  unix_send(); 
extern int  unix_sendmsg(); 
extern int  unix_sendto(); 
extern int  unix_setsockopt(); 
extern int  unix_shutdown(); 
extern int  unix_socket(); 
extern int  unix_traversal(); 
extern int  __unix_rmsg_proc(); 
extern int  __unix_smsg_proc(); 
extern int  __unix_smsg_unproc(); 
extern int  ethernet_vlan_check_hook(); 
extern int  ethernet_vlan_get(); 
extern int  ethernet_vlan_set(); 
extern int  ethernet_vlan_set_hook(); 
extern int  ethernet_vlan_total(); 
extern int  ethernet_vlan_traversal(); 
extern int  call_commit_handler(); 
extern int  get_wireless_stats(); 
extern int  iw_handler_get_iwstats(); 
extern int  wext_handle_ioctl(); 
extern int  wireless_send_event(); 
extern int  ioctl_private_call(); 
extern int  iw_handler_get_private(); 
extern int  iw_handler_get_spy(); 
extern int  iw_handler_get_thrspy(); 
extern int  iw_handler_set_spy(); 
extern int  iw_handler_set_thrspy(); 
extern int  wireless_spy_update(); 
extern int  aio_cancel(); 
extern int  aio_error(); 
extern int  aio_fsync(); 
extern int  aio_read(); 
extern int  aio_return(); 
extern int  aio_suspend(); 
extern int  aio_write(); 
extern int  lio_listio(); 
extern int  __aioAttachWait(); 
extern int  __aioAttachWaitNoCheck(); 
extern int  __aioCancelFd(); 
extern int  __aioCreateWaitChain(); 
extern int  __aioDeleteWaitChain(); 
extern int  __aioDetachWait(); 
extern int  __aioEnqueue(); 
extern int  __aioRemoveAiocb(); 
extern int  __aioSearchFd(); 
extern int  __aioSigevent(); 
extern int  __aioWaitChainSetCnt(); 
extern int  _posixAioInit(); 
extern int  aio_getstacksize(); 
extern int  aio_setstacksize(); 
extern int  dladdr(); 
extern int  dlclose(); 
extern int  dlerror(); 
extern int  dlopen(); 
extern int  dlrefresh(); 
extern int  dlsym(); 
extern int  backtrace_symbols(); 
extern int  backtrace_symbols_fd(); 
extern int  fmtmsg(); 
extern int  fnmatch(); 
extern int  global_std_get(); 
extern int  global_std_set(); 
extern int  register_device(); 
extern int  register_driver(); 
extern int  unregister_device(); 
extern int  unregister_driver(); 
extern int  gjb_cat(); 
extern int  gjb_format(); 
extern int  gjb_mount(); 
extern int  gjb_umount(); 
extern int  int_disable_pic(); 
extern int  int_enable_pic(); 
extern int  int_lock(); 
extern int  int_unlock(); 
extern int  heap_mem_init(); 
extern int  mem_findmax(); 
extern int  mem_getinfo(); 
extern int  mem_show(); 
extern int  mpart_addmem(); 
extern int  mpart_alloc(); 
extern int  mpart_create(); 
extern int  mpart_delete(); 
extern int  mpart_findmaxfree(); 
extern int  mpart_free(); 
extern int  mpart_getinfo(); 
extern int  mpart_memalign(); 
extern int  mpart_module_init(); 
extern int  mpart_realloc(); 
extern int  if_show(); 
extern int  ifconfig(); 
extern int  mbuf_show(); 
extern int  routes_show(); 
extern int  tcp_show(); 
extern int  udp_show(); 
extern int  sys_clk_rate_get(); 
extern int  sys_clk_rate_set(); 
extern int  tick_get(); 
extern int  tick_set(); 
extern int  timer_getinfo(); 
extern int  wdg_cancel(); 
extern int  wdg_create(); 
extern int  wdg_delete(); 
extern int  wdg_getinfo(); 
extern int  wdg_show(); 
extern int  wdg_start(); 
extern int  mlock(); 
extern int  mlockall(); 
extern int  mmap(); 
extern int  mmap64(); 
extern int  mprotect(); 
extern int  mremap(); 
extern int  msync(); 
extern int  munlock(); 
extern int  munlockall(); 
extern int  munmap(); 
extern int  posix_madvise(); 
extern int  shm_open(); 
extern int  shm_unlink(); 
extern int  mq_close(); 
extern int  mq_create(); 
extern int  mq_delete(); 
extern int  mq_getattr(); 
extern int  mq_getinfo(); 
extern int  mq_notify(); 
extern int  mq_open(); 
extern int  mq_open_method(); 
extern int  mq_receive(); 
extern int  mq_reltimedreceive_np(); 
extern int  mq_reltimedsend_np(); 
extern int  mq_send(); 
extern int  mq_setattr(); 
extern int  mq_show(); 
extern int  mq_timedreceive(); 
extern int  mq_timedsend(); 
extern int  mq_unlink(); 
extern int  poll(); 
extern int  ppoll(); 
extern int  __pxnameAdd(); 
extern int  __pxnameDel(); 
extern int  __pxnameDelByNode(); 
extern int  __pxnameGet(); 
extern int  __pxnameSeach(); 
extern int  _posixCtxGet(); 
extern int  _posixCtxTryGet(); 
extern int  _posixVprocCtxGet(); 
extern int  API_PosixInit(); 
extern int  __tshellPosixInit(); 
extern int  __procFsPosixInfoInit(); 
extern int  pthread_atfork(); 
extern int  pthread_cancel(); 
extern int  pthread_cancelforce(); 
extern int  pthread_cleanup_pop(); 
extern int  pthread_cleanup_push(); 
extern int  pthread_close_hook_add(); 
extern int  pthread_close_hook_delete(); 
extern int  pthread_create(); 
extern int  pthread_create_hook_add(); 
extern int  pthread_create_hook_delete(); 
extern int  pthread_delay(); 
extern int  pthread_detach(); 
extern int  pthread_equal(); 
extern int  pthread_exit(); 
extern int  pthread_getaffinity_np(); 
extern int  pthread_getconcurrency(); 
extern int  pthread_getcpuclockid(); 
extern int  pthread_getid(); 
extern int  pthread_getinfo(); 
extern int  pthread_getname(); 
extern int  pthread_getname_np(); 
extern int  pthread_getregs(); 
extern int  pthread_getschedparam(); 
extern int  pthread_getschedprio(); 
extern int  pthread_int_lock_np(); 
extern int  pthread_int_unlock_np(); 
extern int  pthread_is_ready(); 
extern int  pthread_is_suspend(); 
extern int  pthread_join(); 
extern int  pthread_kill(); 
extern int  pthread_list_np(); 
extern int  pthread_lock(); 
extern int  pthread_null_attr_method_np(); 
extern int  pthread_once(); 
extern int  pthread_resume(); 
extern int  pthread_safe_np(); 
extern int  pthread_self(); 
extern int  pthread_setaffinity_np(); 
extern int  pthread_setcancelstate(); 
extern int  pthread_setcanceltype(); 
extern int  pthread_setconcurrency(); 
extern int  pthread_setname_np(); 
extern int  pthread_setschedparam(); 
extern int  pthread_setschedprio(); 
extern int  pthread_show(); 
extern int  pthread_showstack(); 
extern int  pthread_showstackframe(); 
extern int  pthread_sigmask(); 
extern int  pthread_start_np(); 
extern int  pthread_suspend(); 
extern int  pthread_switch_hook_add(); 
extern int  pthread_switch_hook_delete(); 
extern int  pthread_testcancel(); 
extern int  pthread_unlock(); 
extern int  pthread_unsafe_np(); 
extern int  pthread_verify_np(); 
extern int  pthread_verifyid(); 
extern int  pthread_wakeup_np(); 
extern int  pthread_yield(); 
extern int  pthread_attr_destroy(); 
extern int  pthread_attr_get_np(); 
extern int  pthread_attr_getbreakallowed(); 
extern int  pthread_attr_getdetachstate(); 
extern int  pthread_attr_getfpallowed(); 
extern int  pthread_attr_getguardsize(); 
extern int  pthread_attr_getinheritsched(); 
extern int  pthread_attr_getinitonly_np(); 
extern int  pthread_attr_getname(); 
extern int  pthread_attr_getschedparam(); 
extern int  pthread_attr_getschedpolicy(); 
extern int  pthread_attr_getscope(); 
extern int  pthread_attr_getstack(); 
extern int  pthread_attr_getstackaddr(); 
extern int  pthread_attr_getstackfilled(); 
extern int  pthread_attr_getstacksize(); 
extern int  pthread_attr_init(); 
extern int  pthread_attr_setbreakallowed(); 
extern int  pthread_attr_setdetachstate(); 
extern int  pthread_attr_setfpallowed(); 
extern int  pthread_attr_setguardsize(); 
extern int  pthread_attr_setinheritsched(); 
extern int  pthread_attr_setinitonly_np(); 
extern int  pthread_attr_setname(); 
extern int  pthread_attr_setschedparam(); 
extern int  pthread_attr_setschedpolicy(); 
extern int  pthread_attr_setscope(); 
extern int  pthread_attr_setstack(); 
extern int  pthread_attr_setstackaddr(); 
extern int  pthread_attr_setstackfilled(); 
extern int  pthread_attr_setstacksize(); 
extern int  pthread_getattr_np(); 
extern int  pthread_barrier_destroy(); 
extern int  pthread_barrier_init(); 
extern int  pthread_barrier_wait(); 
extern int  pthread_barrierattr_destroy(); 
extern int  pthread_barrierattr_getpshared(); 
extern int  pthread_barrierattr_init(); 
extern int  pthread_barrierattr_setpshared(); 
extern int  _posixPCondInit(); 
extern int  pthread_cond_broadcast(); 
extern int  pthread_cond_destroy(); 
extern int  pthread_cond_getinfo(); 
extern int  pthread_cond_init(); 
extern int  pthread_cond_reltimedwait_np(); 
extern int  pthread_cond_show(); 
extern int  pthread_cond_signal(); 
extern int  pthread_cond_timedwait(); 
extern int  pthread_cond_wait(); 
extern int  pthread_condattr_destroy(); 
extern int  pthread_condattr_getclock(); 
extern int  pthread_condattr_getpshared(); 
extern int  pthread_condattr_init(); 
extern int  pthread_condattr_setclock(); 
extern int  pthread_condattr_setpshared(); 
extern int  _PthreadKeyCleanup(); 
extern int  pthread_getspecific(); 
extern int  pthread_key_cleanup_np(); 
extern int  pthread_key_create(); 
extern int  pthread_key_delete(); 
extern int  pthread_setspecific(); 
extern int  __pthread_mutex_init_invisible(); 
extern int  _posixPMutexInit(); 
extern int  pthread_mutex_destroy(); 
extern int  pthread_mutex_getinfo(); 
extern int  pthread_mutex_getprioceiling(); 
extern int  pthread_mutex_init(); 
extern int  pthread_mutex_lock(); 
extern int  pthread_mutex_reltimedlock_np(); 
extern int  pthread_mutex_setprioceiling(); 
extern int  pthread_mutex_show(); 
extern int  pthread_mutex_timedlock(); 
extern int  pthread_mutex_trylock(); 
extern int  pthread_mutex_unlock(); 
extern int  pthread_mutexattr_destroy(); 
extern int  pthread_mutexattr_getcancelsafe(); 
extern int  pthread_mutexattr_getprioceiling(); 
extern int  pthread_mutexattr_getprotocol(); 
extern int  pthread_mutexattr_getpshared(); 
extern int  pthread_mutexattr_gettype(); 
extern int  pthread_mutexattr_getwaitqtype(); 
extern int  pthread_mutexattr_init(); 
extern int  pthread_mutexattr_setcancelsafe(); 
extern int  pthread_mutexattr_setprioceiling(); 
extern int  pthread_mutexattr_setprotocol(); 
extern int  pthread_mutexattr_setpshared(); 
extern int  pthread_mutexattr_settype(); 
extern int  pthread_mutexattr_setwaitqtype(); 
extern int  _posixPRWLockInit(); 
extern int  pthread_rwlock_destroy(); 
extern int  pthread_rwlock_getinfo(); 
extern int  pthread_rwlock_init(); 
extern int  pthread_rwlock_rdlock(); 
extern int  pthread_rwlock_reltimedrdlock_np(); 
extern int  pthread_rwlock_reltimedwrlock_np(); 
extern int  pthread_rwlock_timedrdlock(); 
extern int  pthread_rwlock_timedwrlock(); 
extern int  pthread_rwlock_tryrdlock(); 
extern int  pthread_rwlock_trywrlock(); 
extern int  pthread_rwlock_unlock(); 
extern int  pthread_rwlock_wrlock(); 
extern int  pthread_rwlockattr_destroy(); 
extern int  pthread_rwlockattr_getkind_np(); 
extern int  pthread_rwlockattr_getpshared(); 
extern int  pthread_rwlockattr_init(); 
extern int  pthread_rwlockattr_setkind_np(); 
extern int  pthread_rwlockattr_setpshared(); 
extern int  pthread_spin_destroy(); 
extern int  pthread_spin_init(); 
extern int  pthread_spin_lock(); 
extern int  pthread_spin_lock_irq_np(); 
extern int  pthread_spin_trylock(); 
extern int  pthread_spin_trylock_irq_np(); 
extern int  pthread_spin_unlock(); 
extern int  pthread_spin_unlock_irq_np(); 
extern int  getpriority(); 
extern int  getrlimit(); 
extern int  getrusage(); 
extern int  nice(); 
extern int  setpriority(); 
extern int  setrlimit(); 
extern int  sched_cpuaffinity_disable_np(); 
extern int  sched_cpuaffinity_enable_np(); 
extern int  sched_cpuaffinity_get_np(); 
extern int  sched_cpuaffinity_set_np(); 
extern int  sched_rms_destroy(); 
extern int  sched_rms_init(); 
extern int  sched_rms_period(); 
extern int  sched_get_priority_max(); 
extern int  sched_get_priority_min(); 
extern int  sched_getaffinity(); 
extern int  sched_getparam(); 
extern int  sched_getscheduler(); 
extern int  sched_gettimeslice(); 
extern int  sched_rr_get_interval(); 
extern int  sched_setaffinity(); 
extern int  sched_setparam(); 
extern int  sched_setscheduler(); 
extern int  sched_settimeslice(); 
extern int  sched_yield(); 
extern int  _posixPSemInit(); 
extern int  sem_close(); 
extern int  sem_destroy(); 
extern int  sem_flush(); 
extern int  sem_getinfo(); 
extern int  sem_getvalue(); 
extern int  sem_init(); 
extern int  sem_open(); 
extern int  sem_open_method(); 
extern int  sem_post(); 
extern int  sem_reltimedwait_np(); 
extern int  sem_show(); 
extern int  sem_timedwait(); 
extern int  sem_trywait(); 
extern int  sem_unlink(); 
extern int  sem_wait(); 
extern int  posix_spawn(); 
extern int  posix_spawn_file_actions_addclose(); 
extern int  posix_spawn_file_actions_adddup2(); 
extern int  posix_spawn_file_actions_addopen(); 
extern int  posix_spawn_file_actions_destroy(); 
extern int  posix_spawn_file_actions_init(); 
extern int  posix_spawnattr_destroy(); 
extern int  posix_spawnattr_getextflags(); 
extern int  posix_spawnattr_getflags(); 
extern int  posix_spawnattr_getopt(); 
extern int  posix_spawnattr_getpgroup(); 
extern int  posix_spawnattr_getschedparam(); 
extern int  posix_spawnattr_getschedpolicy(); 
extern int  posix_spawnattr_getsigdefault(); 
extern int  posix_spawnattr_getsigmask(); 
extern int  posix_spawnattr_getwd(); 
extern int  posix_spawnattr_init(); 
extern int  posix_spawnattr_setextflags(); 
extern int  posix_spawnattr_setflags(); 
extern int  posix_spawnattr_setopt(); 
extern int  posix_spawnattr_setpgroup(); 
extern int  posix_spawnattr_setschedparam(); 
extern int  posix_spawnattr_setschedpolicy(); 
extern int  posix_spawnattr_setsigdefault(); 
extern int  posix_spawnattr_setsigmask(); 
extern int  posix_spawnattr_setwd(); 
extern int  posix_spawnp(); 
extern int  fpathconf(); 
extern int  pathconf(); 
extern int  sysconf(); 
extern int  _posixSyslogInit(); 
extern int  closelog(); 
extern int  openlog(); 
extern int  setlogmask(); 
extern int  setlogmask_r(); 
extern int  syslog(); 
extern int  syslog_r(); 
extern int  adjtime(); 
extern int  ftime(); 
extern int  times(); 
extern int  gethostname(); 
extern int  sethostname(); 
extern int  uname(); 
extern int  __tshellFsCmdInit(); 
extern int  getopt(); 
extern int  getopt_long(); 
extern int  getopt_long_only(); 
extern int  __tShellOptDeleteHook(); 
extern int  __tshellOptEMsg(); 
extern int  __tshellOptNonoptEnd(); 
extern int  __tshellOptNonoptStart(); 
extern int  __tshellOptPlace(); 
extern int  API_TShellOptArg(); 
extern int  API_TShellOptErr(); 
extern int  API_TShellOptFree(); 
extern int  API_TShellOptInd(); 
extern int  API_TShellOptOpt(); 
extern int  API_TShellOptReset(); 
extern int  __hashHorner(); 
extern int  __tshellHeapCmdInit(); 
extern int  __tshellModemCmdInit(); 
extern int  __tshellPerfCmdInit(); 
extern int  __tshellTarCmdInit(); 
extern int  API_TShellCreate(); 
extern int  API_TShellCreateEx(); 
extern int  API_TShellExec(); 
extern int  API_TShellExecBg(); 
extern int  API_TShellFlushCache(); 
extern int  API_TShellFormatAdd(); 
extern int  API_TShellGetGrpName(); 
extern int  API_TShellGetOption(); 
extern int  API_TShellGetUserHome(); 
extern int  API_TShellGetUserName(); 
extern int  API_TShellHelpAdd(); 
extern int  API_TShellHookSet(); 
extern int  API_TShellInit(); 
extern int  API_TShellKeywordAdd(); 
extern int  API_TShellKeywordAddEx(); 
extern int  API_TShellLogout(); 
extern int  API_TShellScrClear(); 
extern int  API_TShellSetOption(); 
extern int  API_TShellSetStackSize(); 
extern int  API_TShellSetTitle(); 
extern int  API_TShellSigEvent(); 
extern int  API_TShellStartup(); 
extern int  API_TShellTermAlert(); 
extern int  __tshellColorInit(); 
extern int  API_TShellColorEnd(); 
extern int  API_TShellColorGet(); 
extern int  API_TShellColorRefresh(); 
extern int  API_TShellColorStart(); 
extern int  API_TShellColorStart2(); 
extern int  __tshellBgCreateEx(); 
extern int  __tshellExec(); 
extern int  __tshellKeywordAdd(); 
extern int  __tshellKeywordFind(); 
extern int  __tshellKeywordList(); 
extern int  __tshellPreTreatedBg(); 
extern int  __tshellRestartEx(); 
extern int  __tshellShowPrompt(); 
extern int  __tshellThread(); 
extern int  __tshellThreadHook(); 
extern int  __tshellUndef(); 
extern int  __tshellHistoryBackup(); 
extern int  __tshellReadline(); 
extern int  __tshellReadlineClean(); 
extern int  __tshellSysCmdInit(); 
extern int  __tshellSysVarInit(); 
extern int  __tshellStrConvertVar(); 
extern int  __tshellStrDelCRLF(); 
extern int  __tshellStrDelTransChar(); 
extern int  __tshellStrFormat(); 
extern int  __tshellStrGetToken(); 
extern int  __tshellStrKeyword(); 
extern int  __tshellUserCmdInit(); 
extern int  __tshellUserAuthen(); 
extern int  __tshellFlushCache(); 
extern int  __tshellGetGrpName(); 
extern int  __tshellGetUserName(); 
extern int  API_TShellVarDelete(); 
extern int  API_TShellVarDup(); 
extern int  API_TShellVarGet(); 
extern int  API_TShellVarGetNum(); 
extern int  API_TShellVarGetRt(); 
extern int  API_TShellVarHookSet(); 
extern int  API_TShellVarLoad(); 
extern int  API_TShellVarSave(); 
extern int  API_TShellVarSet(); 
extern int  __tshellVarAdd(); 
extern int  __tshellVarDefine(); 
extern int  __tshellVarDelete(); 
extern int  __tshellVarDeleteByName(); 
extern int  __tshellVarDup(); 
extern int  __tshellVarFind(); 
extern int  __tshellVarGet(); 
extern int  __tshellVarGetRt(); 
extern int  __tshellVarList(); 
extern int  __tshellVarLoad(); 
extern int  __tshellVarNum(); 
extern int  __tshellVarSave(); 
extern int  __tshellVarSet(); 
extern int  __symbolAddBsp(); 
extern int  __symbolAddSmp(); 
extern int  __symbolAddLibc(); 
extern int  __procFsSymbolInit(); 
extern int  __symbolFindHookSet(); 
extern int  __symbolTraverseHookSet(); 
extern int  API_SymbolAdd(); 
extern int  API_SymbolAddStatic(); 
extern int  API_SymbolDelete(); 
extern int  API_SymbolFind(); 
extern int  API_SymbolInit(); 
extern int  API_SymbolTraverse(); 
extern int  __busAdapterCreate(); 
extern int  __busAdapterDelete(); 
extern int  __busAdapterGet(); 
extern int  __busSystemInit(); 
extern int  API_BusShow(); 
extern int  API_AhciCtrlCreate(); 
extern int  API_AhciCtrlFree(); 
extern int  API_AhciDiskAtaParamGet(); 
extern int  API_AhciDiskAtapiParamGet(); 
extern int  API_AhciDiskCommandSend(); 
extern int  API_AhciDiskTrimSet(); 
extern int  API_AhciNoDataCommandSend(); 
extern int  API_AhciSwapBufLe16(); 
extern int  API_AhciCtrlAdd(); 
extern int  API_AhciCtrlCountGet(); 
extern int  API_AhciCtrlDelete(); 
extern int  API_AhciCtrlDrvDel(); 
extern int  API_AhciCtrlDrvUpdate(); 
extern int  API_AhciCtrlHandleGetFromIndex(); 
extern int  API_AhciCtrlHandleGetFromName(); 
extern int  API_AhciCtrlHandleGetFromPciArg(); 
extern int  API_AhciCtrlIndexGet(); 
extern int  API_AhciCtrlInit(); 
extern int  API_AhciDevAdd(); 
extern int  API_AhciDevCountGet(); 
extern int  API_AhciDevDelete(); 
extern int  API_AhciDevHandleGet(); 
extern int  API_AhciDevInit(); 
extern int  API_AhciDevIoctl(); 
extern int  API_AhciDrvCtrlAdd(); 
extern int  API_AhciDrvCtrlDelete(); 
extern int  API_AhciDrvCtrlFind(); 
extern int  API_AhciDrvDelete(); 
extern int  API_AhciDrvHandleGet(); 
extern int  API_AhciDrvInit(); 
extern int  API_AhciDrvRegister(); 
extern int  API_AhciCtrlAhciModeEnable(); 
extern int  API_AhciCtrlCapGet(); 
extern int  API_AhciCtrlImpPortGet(); 
extern int  API_AhciCtrlInfoShow(); 
extern int  API_AhciCtrlIntConnect(); 
extern int  API_AhciCtrlRegisterWait(); 
extern int  API_AhciCtrlRegNameGet(); 
extern int  API_AhciCtrlReset(); 
extern int  API_AhciCtrlSssSet(); 
extern int  API_AhciDriveEngineStop(); 
extern int  API_AhciDriveFwRevInfoGet(); 
extern int  API_AhciDriveInfoShow(); 
extern int  API_AhciDriveModelInfoGet(); 
extern int  API_AhciDrivePowerUp(); 
extern int  API_AhciDriveRecvFisStop(); 
extern int  API_AhciDriveRegNameGet(); 
extern int  API_AhciDriveRegWait(); 
extern int  API_AhciDriveSectorCountGet(); 
extern int  API_AhciDriveSerialInfoGet(); 
extern int  API_AhciDriveWorkModeNameGet(); 
extern int  API_AhciApmDisable(); 
extern int  API_AhciApmEnable(); 
extern int  API_AhciApmModeGet(); 
extern int  API_AhciPmActive(); 
extern int  API_AhciPmPowerModeGet(); 
extern int  API_AtaCtrlIrqIsr(); 
extern int  API_AtaCtrlStatusCheck(); 
extern int  API_AtaDrvInit(); 
extern int  ataCtrlDelay(); 
extern int  ataDriveCommandSend(); 
extern int  ataDriveFwRevInfoGet(); 
extern int  ataDriveInit(); 
extern int  ataDriveModelInfoGet(); 
extern int  ataDriveSerialInfoGet(); 
extern int  ataDrvInit(); 
extern int  ataSwapBufLe16(); 
extern int  __blockIoDevCreate(); 
extern int  __blockIoDevDelete(); 
extern int  __blockIoDevFlag(); 
extern int  __blockIoDevGet(); 
extern int  __blockIoDevInit(); 
extern int  __blockIoDevIoctl(); 
extern int  __blockIoDevIsLogic(); 
extern int  __blockIoDevRead(); 
extern int  __blockIoDevReset(); 
extern int  __blockIoDevStatus(); 
extern int  __blockIoDevWrite(); 
extern int  API_BlkRawCreate(); 
extern int  API_BlkRawCreateEx(); 
extern int  API_BlkRawDelete(); 
extern int  API_RamDiskCreate(); 
extern int  API_RamDiskDelete(); 
extern int  API_BmsgDevCreate(); 
extern int  API_BmsgDrvInstall(); 
extern int  bmsgfd(); 
extern int  bmsgfd_bind(); 
extern int  bmsgfd_timeout(); 
extern int  bmsgfd_unbind(); 
extern int  API_BuzzerDevCreate(); 
extern int  API_BuzzerDrvInstall(); 
extern int  API_CanDevCreate(); 
extern int  API_CanDevRemove(); 
extern int  API_CanDrvInstall(); 
extern int  API_DmaContext(); 
extern int  API_DmaDrvInstall(); 
extern int  API_DmaFlush(); 
extern int  API_DmaGetMaxDataBytes(); 
extern int  API_DmaJobAdd(); 
extern int  API_DmaJobNodeNum(); 
extern int  API_DmaMaxNodeNumGet(); 
extern int  API_DmaMaxNodeNumSet(); 
extern int  API_DmaReset(); 
extern int  _dmaDeleteFromWaitList(); 
extern int  _dmaGetFirstInWaitList(); 
extern int  _dmaInit(); 
extern int  _dmaInsertToWaitList(); 
extern int  _dmaWaitnodeAlloc(); 
extern int  _dmaWaitnodeFree(); 
extern int  API_EventfdDevCreate(); 
extern int  API_EventfdDrvInstall(); 
extern int  eventfd(); 
extern int  eventfd_read(); 
extern int  eventfd_write(); 
extern int  API_GpiofdDevCreate(); 
extern int  API_GpiofdDrvInstall(); 
extern int  gpiofd(); 
extern int  gpiofd_read(); 
extern int  gpiofd_write(); 
extern int  API_GpioChipAdd(); 
extern int  API_GpioChipDelete(); 
extern int  API_GpioChipFind(); 
extern int  API_GpioClearIrq(); 
extern int  API_GpioDirectionInput(); 
extern int  API_GpioDirectionOutput(); 
extern int  API_GpioFree(); 
extern int  API_GpioFreeArray(); 
extern int  API_GpioGetFlags(); 
extern int  API_GpioGetIrq(); 
extern int  API_GpioGetValue(); 
extern int  API_GpioHasDrv(); 
extern int  API_GpioInit(); 
extern int  API_GpioIsValid(); 
extern int  API_GpioOpenDrain(); 
extern int  API_GpioOpenSource(); 
extern int  API_GpioRequest(); 
extern int  API_GpioRequestArray(); 
extern int  API_GpioRequestOne(); 
extern int  API_GpioSetDebounce(); 
extern int  API_GpioSetPull(); 
extern int  API_GpioSetupIrq(); 
extern int  API_GpioSetValue(); 
extern int  API_GpioSvrIrq(); 
extern int  API_GMemDevAdd(); 
extern int  API_GMemDrawHLine(); 
extern int  API_GMemDrawVLine(); 
extern int  API_GMemFillRect(); 
extern int  API_GMemGet2D(); 
extern int  API_GMemGetPalette(); 
extern int  API_GMemGetPixel(); 
extern int  API_GMemSetAlpha(); 
extern int  API_GMemSetColor(); 
extern int  API_GMemSetPalette(); 
extern int  API_GMemSetPixel(); 
extern int  API_HstimerfdDevCreate(); 
extern int  API_HstimerfdDrvInstall(); 
extern int  hstimerfd_create(); 
extern int  hstimerfd_gettime(); 
extern int  hstimerfd_gettime2(); 
extern int  hstimerfd_hz(); 
extern int  hstimerfd_settime(); 
extern int  hstimerfd_settime2(); 
extern int  API_RtcDevCreate(); 
extern int  API_RtcDrvInstall(); 
extern int  API_RtcGet(); 
extern int  API_RtcSet(); 
extern int  API_RtcToRoot(); 
extern int  API_RtcToSys(); 
extern int  API_SysToRtc(); 
extern int  API_I2cAdapterCreate(); 
extern int  API_I2cAdapterDelete(); 
extern int  API_I2cAdapterGet(); 
extern int  API_I2cDeviceCreate(); 
extern int  API_I2cDeviceCtl(); 
extern int  API_I2cDeviceDelete(); 
extern int  API_I2cDeviceMasterRecv(); 
extern int  API_I2cDeviceMasterSend(); 
extern int  API_I2cDeviceTransfer(); 
extern int  API_I2cDeviceUsageDec(); 
extern int  API_I2cDeviceUsageGet(); 
extern int  API_I2cDeviceUsageInc(); 
extern int  API_I2cLibInit(); 
extern int  API_MemDevCreate(); 
extern int  API_MemDevCreateDir(); 
extern int  API_MemDevDelete(); 
extern int  API_MemDrvInstall(); 
extern int  API_MiiLibInit(); 
extern int  API_MiiPhyAdd(); 
extern int  API_MiiPhyDel(); 
extern int  API_MiiPhyDiagnostic(); 
extern int  API_MiiPhyInit(); 
extern int  API_MiiPhyIsolate(); 
extern int  API_MiiPhyLinkSet(); 
extern int  API_MiiPhyLinkStatGet(); 
extern int  API_MiiPhyModeSet(); 
extern int  API_MiiPhyMonitorStart(); 
extern int  API_MiiPhyMonitorStop(); 
extern int  API_MiiPhyProbe(); 
extern int  API_MiiPhyScan(); 
extern int  API_MiiPhyScanOnly(); 
extern int  API_NvmeCtrlCreate(); 
extern int  API_NvmeCtrlFree(); 
extern int  API_NvmeCtrlAdd(); 
extern int  API_NvmeCtrlCountGet(); 
extern int  API_NvmeCtrlDelete(); 
extern int  API_NvmeCtrlHandleGetFromIndex(); 
extern int  API_NvmeCtrlHandleGetFromName(); 
extern int  API_NvmeCtrlHandleGetFromPciArg(); 
extern int  API_NvmeCtrlIndexGet(); 
extern int  API_NvmeCtrlInit(); 
extern int  __nvmeMonitorThread(); 
extern int  API_NvmeDevAdd(); 
extern int  API_NvmeDevCountGet(); 
extern int  API_NvmeDevDelete(); 
extern int  API_NvmeDevHandleGet(); 
extern int  API_NvmeDevInit(); 
extern int  API_NvmeDrvHandleGet(); 
extern int  API_NvmeDrvInit(); 
extern int  API_NvmeDrvRegister(); 
extern int  API_NvmeCtrlIntConnect(); 
extern int  API_PciAutoBusReset(); 
extern int  API_PciAutoBusScan(); 
extern int  API_PciAutoCtrlInit(); 
extern int  API_PciAutoCtrlRegionSet(); 
extern int  API_PciAutoDeviceConfig(); 
extern int  API_PciAutoDeviceSetup(); 
extern int  API_PciAutoPostScanBridgeSetup(); 
extern int  API_PciAutoPreScanBridgeSetup(); 
extern int  API_PciAutoRegionAlign(); 
extern int  API_PciAutoRegionAllocate(); 
extern int  API_PciAutoRegionInit(); 
extern int  API_PciAutoScan(); 
extern int  API_PciCapEn(); 
extern int  API_PciCapFind(); 
extern int  API_PciCapShow(); 
extern int  API_PciCapExtFind(); 
extern int  API_PciCapExtShow(); 
extern int  API_PciDbGetClassStr(); 
extern int  API_PciDbGetIdStr(); 
extern int  API_PciDbInit(); 
extern int  API_PciDevAdd(); 
extern int  API_PciDevConfigRead(); 
extern int  API_PciDevConfigReadByte(); 
extern int  API_PciDevConfigReadDword(); 
extern int  API_PciDevConfigReadWord(); 
extern int  API_PciDevConfigWrite(); 
extern int  API_PciDevConfigWriteByte(); 
extern int  API_PciDevConfigWriteDword(); 
extern int  API_PciDevConfigWriteWord(); 
extern int  API_PciDevDelete(); 
extern int  API_PciDevDrvDel(); 
extern int  API_PciDevDrvUpdate(); 
extern int  API_PciDevHandleGet(); 
extern int  API_PciDevInit(); 
extern int  API_PciDevInterConnect(); 
extern int  API_PciDevInterDisable(); 
extern int  API_PciDevInterDisableEx(); 
extern int  API_PciDevInterDisconnect(); 
extern int  API_PciDevInterEnable(); 
extern int  API_PciDevInterMsiGet(); 
extern int  API_PciDevInterServiceCnt(); 
extern int  API_PciDevInterVectorGet(); 
extern int  API_PciDevIntxEnableSet(); 
extern int  API_PciDevIoRemap(); 
extern int  API_PciDevIoRemap2(); 
extern int  API_PciDevIoRemapEx(); 
extern int  API_PciDevIoRemapEx2(); 
extern int  API_PciDevIoUnmap(); 
extern int  API_PciDevListCreate(); 
extern int  API_PciDevMasterEnable(); 
extern int  API_PciDevParentHandleGet(); 
extern int  API_PciDevSetupAll(); 
extern int  API_PciDrvBindEachDev(); 
extern int  API_PciDevBindEachDrv(); 
extern int  API_PciDrvDevAdd(); 
extern int  API_PciDrvDevDel(); 
extern int  API_PciDrvDevFind(); 
extern int  API_PciDrvHandleGet(); 
extern int  API_PciDrvInit(); 
extern int  API_PciDrvLoad(); 
extern int  API_PciDrvRegister(); 
extern int  API_PciDrvUnregister(); 
extern int  API_PciConfigBusMaxGet(); 
extern int  API_PciConfigBusMaxSet(); 
extern int  API_PciConfigFetch(); 
extern int  API_PciConfigHandleGet(); 
extern int  API_PciConfigInByte(); 
extern int  API_PciConfigIndexGet(); 
extern int  API_PciConfigInDword(); 
extern int  API_PciConfigInWord(); 
extern int  API_PciConfigModifyByte(); 
extern int  API_PciConfigModifyDword(); 
extern int  API_PciConfigModifyWord(); 
extern int  API_PciConfigOutByte(); 
extern int  API_PciConfigOutDword(); 
extern int  API_PciConfigOutWord(); 
extern int  API_PciCtrlCreate(); 
extern int  API_PciCtrlReset(); 
extern int  API_PciDevMatchDrv(); 
extern int  API_PciDevMemConfig(); 
extern int  API_PciDevMemSizeGet(); 
extern int  API_PciDevResourceGet(); 
extern int  API_PciDevSetup(); 
extern int  API_PciDevStdResourceFind(); 
extern int  API_PciDevStdResourceGet(); 
extern int  API_PciFindClass(); 
extern int  API_PciFindDev(); 
extern int  API_PciFuncDisable(); 
extern int  API_PciGetHeader(); 
extern int  API_PciHeaderTypeGet(); 
extern int  API_PciInterConnect(); 
extern int  API_PciInterDisconnect(); 
extern int  API_PciIntxEnableSet(); 
extern int  API_PciIntxMaskSupported(); 
extern int  API_PciIrqGet(); 
extern int  API_PciIrqMsi(); 
extern int  API_PciResourceGet(); 
extern int  API_PciSizeNameGet(); 
extern int  API_PciSizeNumGet(); 
extern int  API_PciSpecialCycle(); 
extern int  API_PciTraversal(); 
extern int  API_PciTraversalDev(); 
extern int  API_PciVpdRead(); 
extern int  API_PciDevMsiEnableGet(); 
extern int  API_PciDevMsiEnableSet(); 
extern int  API_PciDevMsiRangeEnable(); 
extern int  API_PciDevMsiVecCountGet(); 
extern int  API_PciMsi64BitGet(); 
extern int  API_PciMsiEnableGet(); 
extern int  API_PciMsiEnableSet(); 
extern int  API_PciMsiMaskBitGet(); 
extern int  API_PciMsiMaskConvert(); 
extern int  API_PciMsiMaskGet(); 
extern int  API_PciMsiMaskPosGet(); 
extern int  API_PciMsiMaskSet(); 
extern int  API_PciMsiMsgRead(); 
extern int  API_PciMsiMsgWrite(); 
extern int  API_PciMsiMultiCapGet(); 
extern int  API_PciMsiMultipleGet(); 
extern int  API_PciMsiPendingGet(); 
extern int  API_PciMsiPendingPosGet(); 
extern int  API_PciMsiPendingSet(); 
extern int  API_PciMsiVecCountGet(); 
extern int  API_PciMsixClearSet(); 
extern int  API_PciDevMsixEnableGet(); 
extern int  API_PciDevMsixEnableSet(); 
extern int  API_PciDevMsixRangeEnable(); 
extern int  API_PciDevMsixVecCountGet(); 
extern int  API_PciMsixEnableGet(); 
extern int  API_PciMsixEnableSet(); 
extern int  API_PciMsixFunctionMaskGet(); 
extern int  API_PciMsixFunctionMaskSet(); 
extern int  API_PciMsixMaskGet(); 
extern int  API_PciMsixMaskSet(); 
extern int  API_PciMsixMsgRead(); 
extern int  API_PciMsixMsgWrite(); 
extern int  API_PciMsixPendingGet(); 
extern int  API_PciMsixPendingPosGet(); 
extern int  API_PciMsixTablePosGet(); 
extern int  API_PciMsixVecCountGet(); 
extern int  __procFsPciInit(); 
extern int  API_PciScan(); 
extern int  API_PciBarShow(); 
extern int  API_PciBistShow(); 
extern int  API_PciBusDeviceShow(); 
extern int  API_PciCapMsiShow(); 
extern int  API_PciCapPcieShow(); 
extern int  API_PciCapPcixShow(); 
extern int  API_PciCommandShow(); 
extern int  API_PciConfigShow(); 
extern int  API_PciFindClassShow(); 
extern int  API_PciFindDevShow(); 
extern int  API_PciHeaderShow(); 
extern int  API_PciIdsShow(); 
extern int  API_PciInterruptShow(); 
extern int  API_PciReversionShow(); 
extern int  API_PciStatusShow(); 
extern int  API_PciCapVpdShow(); 
extern int  API_PipeDevCreate(); 
extern int  API_PipeDevDelete(); 
extern int  API_PipeDrvInstall(); 
extern int  _PipeClose(); 
extern int  _PipeIoctl(); 
extern int  _PipeOpen(); 
extern int  _PipeRead(); 
extern int  _PipeWrite(); 
extern int  API_PtyDevCreate(); 
extern int  API_PtyDevRemove(); 
extern int  API_PtyDrvInstall(); 
extern int  _PtyDeviceClose(); 
extern int  _PtyDeviceIoctl(); 
extern int  _PtyDeviceOpen(); 
extern int  _PtyDeviceRead(); 
extern int  _PtyDeviceStartup(); 
extern int  _PtyDeviceWrite(); 
extern int  _PtyHostClose(); 
extern int  _PtyHostIoctl(); 
extern int  _PtyHostOpen(); 
extern int  _PtyHostRead(); 
extern int  _PtyHostWrite(); 
extern int  API_RandDevCreate(); 
extern int  API_RandDrvInstall(); 
extern int  __randClose(); 
extern int  __randInit(); 
extern int  __randIoctl(); 
extern int  __randOpen(); 
extern int  __randRead(); 
extern int  __randWrite(); 
extern int  API_SdAdapterCreate(); 
extern int  API_SdAdapterDelete(); 
extern int  API_SdAdapterGet(); 
extern int  API_SdDeviceCreate(); 
extern int  API_SdDeviceCtl(); 
extern int  API_SdDeviceDelete(); 
extern int  API_SdDeviceGet(); 
extern int  API_SdDeviceTransfer(); 
extern int  API_SdDeviceUsageDec(); 
extern int  API_SdDeviceUsageGet(); 
extern int  API_SdDeviceUsageInc(); 
extern int  API_SdLibInit(); 
extern int  __sdiobaseDevIrqHandle(); 
extern int  API_SdioBaseDrvInstall(); 
extern int  API_SdMemDevCreate(); 
extern int  API_SdMemDevDelete(); 
extern int  API_SdMemDevShow(); 
extern int  API_SdMemDrvInstall(); 
extern int  API_SdCoreDevAdapterName(); 
extern int  API_SdCoreDevAppCmd(); 
extern int  API_SdCoreDevAppSwitch(); 
extern int  API_SdCoreDevCidSet(); 
extern int  API_SdCoreDevCidView(); 
extern int  API_SdCoreDevCmd(); 
extern int  API_SdCoreDevCreate(); 
extern int  API_SdCoreDevCsdSet(); 
extern int  API_SdCoreDevCsdView(); 
extern int  API_SdCoreDevCtl(); 
extern int  API_SdCoreDevDelete(); 
extern int  API_SdCoreDevRcaSet(); 
extern int  API_SdCoreDevRcaView(); 
extern int  API_SdCoreDevScrSet(); 
extern int  API_SdCoreDevScrView(); 
extern int  API_SdCoreDevStaView(); 
extern int  API_SdCoreDevSwCapSet(); 
extern int  API_SdCoreDevSwCapView(); 
extern int  API_SdCoreDevTransfer(); 
extern int  API_SdCoreDevTypeSet(); 
extern int  API_SdCoreDevTypeView(); 
extern int  API_SdCoreSpiCxdFormat(); 
extern int  API_SdCoreSpiMulWrtStop(); 
extern int  API_SdCoreSpiRegisterRead(); 
extern int  API_SdCoreSpiSendIfCond(); 
extern int  API_SdCoreDecodeCID(); 
extern int  API_SdCoreDecodeCSD(); 
extern int  API_SdCoreDecodeExtCSD(); 
extern int  API_SdCoreDevDeSelect(); 
extern int  API_SdCoreDevGetStatus(); 
extern int  API_SdCoreDevIsBlockAddr(); 
extern int  API_SdCoreDevMmcSetRelativeAddr(); 
extern int  API_SdCoreDevReset(); 
extern int  API_SdCoreDevSelect(); 
extern int  API_SdCoreDevSendAllCID(); 
extern int  API_SdCoreDevSendAllCSD(); 
extern int  API_SdCoreDevSendAllSCR(); 
extern int  API_SdCoreDevSendAppOpCond(); 
extern int  API_SdCoreDevSendExtCSD(); 
extern int  API_SdCoreDevSendIfCond(); 
extern int  API_SdCoreDevSendRelativeAddr(); 
extern int  API_SdCoreDevSetBlkLen(); 
extern int  API_SdCoreDevSetBlkLenRaw(); 
extern int  API_SdCoreDevSetBusWidth(); 
extern int  API_SdCoreDevSetPreBlkLen(); 
extern int  API_SdCoreDevSpiClkDely(); 
extern int  API_SdCoreDevSpiCrcEn(); 
extern int  API_SdCoreDevSwitch(); 
extern int  API_SdCoreDevSwitchEx(); 
extern int  __sdCrc16(); 
extern int  __sdCrcCmdCrc7(); 
extern int  API_SdmEventNotify(); 
extern int  API_SdmHostCapGet(); 
extern int  API_SdmHostExtOptGet(); 
extern int  API_SdmHostExtOptSet(); 
extern int  API_SdmHostGet(); 
extern int  API_SdmHostInterEn(); 
extern int  API_SdmHostIsCardWp(); 
extern int  API_SdmHostRegister(); 
extern int  API_SdmHostUnRegister(); 
extern int  API_SdmLibInit(); 
extern int  API_SdmSdDrvRegister(); 
extern int  API_SdmSdDrvUnRegister(); 
extern int  API_SdioCoreDevFuncBlkSzSet(); 
extern int  API_SdioCoreDevFuncClean(); 
extern int  API_SdioCoreDevFuncDis(); 
extern int  API_SdioCoreDevFuncEn(); 
extern int  API_SdioCoreDevFuncIntDis(); 
extern int  API_SdioCoreDevFuncIntEn(); 
extern int  API_SdioCoreDevHighSpeedEn(); 
extern int  API_SdioCoreDevReadByte(); 
extern int  API_SdioCoreDevReadCCCR(); 
extern int  API_SdioCoreDevReadCis(); 
extern int  API_SdioCoreDevReadFbr(); 
extern int  API_SdioCoreDevReset(); 
extern int  API_SdioCoreDevRwDirect(); 
extern int  API_SdioCoreDevRwExtend(); 
extern int  API_SdioCoreDevRwExtendX(); 
extern int  API_SdioCoreDevSendIoOpCond(); 
extern int  API_SdioCoreDevWideBusEn(); 
extern int  API_SdioCoreDevWriteByte(); 
extern int  API_SdioCoreDevWriteThenReadByte(); 
extern int  __sdmSdioDrvAccessRelease(); 
extern int  __sdmSdioDrvAccessRequest(); 
extern int  __sdmSdioDrvHeader(); 
extern int  API_SdmSdioDrvRegister(); 
extern int  API_SdmSdioDrvUnRegister(); 
extern int  API_SdmSdioLibInit(); 
extern int  __sdUnitGet(); 
extern int  __sdUnitPoolCreate(); 
extern int  __sdUnitPoolDelete(); 
extern int  __sdUnitPut(); 
extern int  API_SdhciDeviceCheckNotify(); 
extern int  API_SdhciDeviceUsageDec(); 
extern int  API_SdhciDeviceUsageGet(); 
extern int  API_SdhciDeviceUsageInc(); 
extern int  API_SdhciHostCreate(); 
extern int  API_SdhciHostDelete(); 
extern int  API_SdhciHostTransferModGet(); 
extern int  API_SdhciHostTransferModSet(); 
extern int  API_SdhciSdmHostGet(); 
extern int  semfd(); 
extern int  semfd_timeout(); 
extern int  API_SemfdDevCreate(); 
extern int  API_SemfdDrvInstall(); 
extern int  API_ShmDevCreate(); 
extern int  API_ShmDrvInstall(); 
extern int  API_SpiAdapterCreate(); 
extern int  API_SpiAdapterDelete(); 
extern int  API_SpiAdapterGet(); 
extern int  API_SpiDeviceBusRelease(); 
extern int  API_SpiDeviceBusRequest(); 
extern int  API_SpiDeviceCreate(); 
extern int  API_SpiDeviceCtl(); 
extern int  API_SpiDeviceDelete(); 
extern int  API_SpiDeviceTransfer(); 
extern int  API_SpiDeviceUsageDec(); 
extern int  API_SpiDeviceUsageGet(); 
extern int  API_SpiDeviceUsageInc(); 
extern int  API_SpiLibInit(); 
extern int  API_SpipeDevCreate(); 
extern int  API_SpipeDevDelete(); 
extern int  API_SpipeDrvInstall(); 
extern int  _SpipeClose(); 
extern int  _SpipeIoctl(); 
extern int  _SpipeOpen(); 
extern int  _SpipeRead(); 
extern int  _SpipeRemove(); 
extern int  _SpipeWrite(); 
extern int  cfgetispeed(); 
extern int  cfgetospeed(); 
extern int  cfmakeraw(); 
extern int  cfsetispeed(); 
extern int  cfsetospeed(); 
extern int  tcdrain(); 
extern int  tcflow(); 
extern int  tcflush(); 
extern int  tcgetattr(); 
extern int  tcgetsid(); 
extern int  tcsendbreak(); 
extern int  tcsetattr(); 
extern int  API_TtyDevCreate(); 
extern int  API_TtyDevRemove(); 
extern int  API_TtyDrvInstall(); 
extern int  _TyDevInit(); 
extern int  _TyDevRemove(); 
extern int  _TyIoctl(); 
extern int  _TyIRd(); 
extern int  _TyITx(); 
extern int  _TyRead(); 
extern int  _TyWrite(); 
extern int  API_TyAbortFuncSet(); 
extern int  API_TyAbortSet(); 
extern int  API_TyBackspaceSet(); 
extern int  API_TyDeleteLineSet(); 
extern int  API_TyEOFSet(); 
extern int  API_TyMonitorTrapSet(); 
extern int  API_SystemHwInfoPrint(); 
extern int  API_SystemLogoPrint(); 
extern int  _epollAddEvent(); 
extern int  _epollDelEvent(); 
extern int  _epollDevCreate(); 
extern int  _epollDrvInstall(); 
extern int  _epollFindEvent(); 
extern int  _epollFiniFdset(); 
extern int  _epollInitFdset(); 
extern int  _epollModEvent(); 
extern int  _EpollInit(); 
extern int  epoll_create(); 
extern int  epoll_create1(); 
extern int  epoll_ctl(); 
extern int  epoll_pwait(); 
extern int  epoll_wait(); 
extern int  _ExcGetLost(); 
extern int  _excInit(); 
extern int  _excJobAdd(); 
extern int  _excJobDel(); 
extern int  _hotplugDevCreate(); 
extern int  _hotplugDevPutMsg(); 
extern int  _hotplugDrvInstall(); 
extern int  _hotplugInit(); 
extern int  API_HotplugContext(); 
extern int  API_HotplugEvent(); 
extern int  API_HotplugEventMessage(); 
extern int  API_HotplugGetLost(); 
extern int  API_HotplugPollAdd(); 
extern int  API_HotplugPollDelete(); 
extern int  closedir(); 
extern int  dirfd(); 
extern int  futimes(); 
extern int  mkdir(); 
extern int  opendir(); 
extern int  readdir(); 
extern int  readdir_r(); 
extern int  readdir64(); 
extern int  readdir64_r(); 
extern int  rewinddir(); 
extern int  rmdir(); 
extern int  utime(); 
extern int  utimes(); 
extern int  fcntl(); 
extern int  API_IosFdNodeAdd(); 
extern int  API_IosFdNodeDec(); 
extern int  API_IosFdNodeFind(); 
extern int  API_IosFdNodeLock(); 
extern int  mkfifo(); 
extern int  pipe(); 
extern int  pipe2(); 
extern int  _IosFileClose(); 
extern int  _IosFileDelete(); 
extern int  _IosFileDescGet(); 
extern int  _IosFileDup(); 
extern int  _IosFileDup2(); 
extern int  _IosFileGet(); 
extern int  _IosFileGetKernel(); 
extern int  _IosFileIoctl(); 
extern int  _IosFileListIslock(); 
extern int  _IosFileListLock(); 
extern int  _IosFileListRemoveReq(); 
extern int  _IosFileListUnlock(); 
extern int  _IosFileNew(); 
extern int  _IosFileRealName(); 
extern int  _IosFileRefDec(); 
extern int  _IosFileRefGet(); 
extern int  _IosFileRefInc(); 
extern int  _IosFileSet(); 
extern int  _IosFileSync(); 
extern int  API_IosFdDevFind(); 
extern int  API_IosFdEntryReclaim(); 
extern int  API_IosFdFree(); 
extern int  API_IosFdGetCloExec(); 
extern int  API_IosFdGetFlag(); 
extern int  API_IosFdGetName(); 
extern int  API_IosFdGetRealName(); 
extern int  API_IosFdGetType(); 
extern int  API_IosFdIsBusy(); 
extern int  API_IosFdLock(); 
extern int  API_IosFdNew(); 
extern int  API_IosFdRealName(); 
extern int  API_IosFdRefDec(); 
extern int  API_IosFdRefInc(); 
extern int  API_IosFdSet(); 
extern int  API_IosFdSetCloExec(); 
extern int  API_IosFdUnlink(); 
extern int  API_IosFdValue(); 
extern int  API_IosFdValueType(); 
extern int  diskformat(); 
extern int  diskinit(); 
extern int  API_IoDefDevGet(); 
extern int  API_IoDefDirGet(); 
extern int  API_IoDefPathCat(); 
extern int  API_IoDefPathGet(); 
extern int  API_IoDefPathSet(); 
extern int  API_IoFullFileNameGet(); 
extern int  API_IoGlobalStdGet(); 
extern int  API_IoGlobalStdSet(); 
extern int  API_IoPathCondense(); 
extern int  API_IoPrivateEnv(); 
extern int  API_IoTaskStdGet(); 
extern int  API_IoTaskStdSet(); 
extern int  cd(); 
extern int  chdir(); 
extern int  chroot(); 
extern int  close(); 
extern int  creat(); 
extern int  dup(); 
extern int  dup2(); 
extern int  dup2kernel(); 
extern int  dupminfd(); 
extern int  fchdir(); 
extern int  fisbusy(); 
extern int  flock(); 
extern int  getcwd(); 
extern int  getwd(); 
extern int  ioctl(); 
extern int  isatty(); 
extern int  llseek(); 
extern int  lockf(); 
extern int  lseek(); 
extern int  lseek64(); 
extern int  major(); 
extern int  makedev(); 
extern int  minor(); 
extern int  mknod(); 
extern int  open(); 
extern int  open64(); 
extern int  pread(); 
extern int  pread64(); 
extern int  pwrite(); 
extern int  pwrite64(); 
extern int  read(); 
extern int  readv(); 
extern int  realpath(); 
extern int  rename(); 
extern int  ttyname(); 
extern int  ttyname_r(); 
extern int  umask(); 
extern int  unlink(); 
extern int  write(); 
extern int  writev(); 
extern int  _IosCheckPermissions(); 
extern int  _IosEnvCreate(); 
extern int  _IosEnvDelete(); 
extern int  _IosEnvGetDef(); 
extern int  _IosEnvInherit(); 
extern int  _IosInit(); 
extern int  _IosLock(); 
extern int  _IosUnlock(); 
extern int  API_IoGetDrvAuthor(); 
extern int  API_IoGetDrvDescription(); 
extern int  API_IoGetDrvLicense(); 
extern int  API_IoSetDrvAuthor(); 
extern int  API_IoSetDrvDescription(); 
extern int  API_IoSetDrvLicense(); 
extern int  __fdLockfCleanupHook(); 
extern int  _FdLockfClearFdEntry(); 
extern int  _FdLockfClearFdNode(); 
extern int  _FdLockfIoctl(); 
extern int  _FdLockfProc(); 
extern int  _PathBuildLink(); 
extern int  _PathCat(); 
extern int  _PathCondense(); 
extern int  _PathGetDef(); 
extern int  _PathGetFull(); 
extern int  _PathLastName(); 
extern int  _PathLastNamePtr(); 
extern int  _PathMoveCheck(); 
extern int  API_IoDevShow(); 
extern int  API_IoDrvLicenseShow(); 
extern int  API_IoDrvShow(); 
extern int  API_IoFdentryShow(); 
extern int  API_IoFdShow(); 
extern int  access(); 
extern int  chmod(); 
extern int  chown(); 
extern int  fchmod(); 
extern int  fchown(); 
extern int  fdatasync(); 
extern int  fstat(); 
extern int  fstat64(); 
extern int  fstatfs(); 
extern int  fsync(); 
extern int  ftruncate(); 
extern int  ftruncate64(); 
extern int  lchown(); 
extern int  lstat(); 
extern int  lstat64(); 
extern int  stat(); 
extern int  stat64(); 
extern int  statfs(); 
extern int  sync(); 
extern int  truncate(); 
extern int  truncate64(); 
extern int  link(); 
extern int  readlink(); 
extern int  symlink(); 
extern int  API_IosClose(); 
extern int  API_IosCreate(); 
extern int  API_IosDelete(); 
extern int  API_IosDevAdd(); 
extern int  API_IosDevAddEx(); 
extern int  API_IosDevDelete(); 
extern int  API_IosDevFileAbnormal(); 
extern int  API_IosDevFind(); 
extern int  API_IosDevMatch(); 
extern int  API_IosDevMatchFull(); 
extern int  API_IosDrvGetType(); 
extern int  API_IosDrvInstall(); 
extern int  API_IosDrvInstallEx(); 
extern int  API_IosDrvInstallEx2(); 
extern int  API_IosDrvRemove(); 
extern int  API_IosFstat(); 
extern int  API_IosIoctl(); 
extern int  API_IosLseek(); 
extern int  API_IosLstat(); 
extern int  API_IosMmap(); 
extern int  API_IosNextDevGet(); 
extern int  API_IosOpen(); 
extern int  API_IosPRead(); 
extern int  API_IosPWrite(); 
extern int  API_IosRead(); 
extern int  API_IosReadlink(); 
extern int  API_IosSymlink(); 
extern int  API_IosUnmap(); 
extern int  API_IosWrite(); 
extern int  __logPrintf(); 
extern int  __logPrintk(); 
extern int  _logInit(); 
extern int  _LogThread(); 
extern int  API_LogFdAdd(); 
extern int  API_LogFdDelete(); 
extern int  API_LogFdGet(); 
extern int  API_LogFdSet(); 
extern int  API_LogMsg(); 
extern int  API_LogPrintk(); 
extern int  API_PowerMAdapterCreate(); 
extern int  API_PowerMAdapterDelete(); 
extern int  API_PowerMAdapterFind(); 
extern int  API_PowerMDevInit(); 
extern int  API_PowerMDevOff(); 
extern int  API_PowerMDevOn(); 
extern int  API_PowerMDevTerm(); 
extern int  _PowerMInit(); 
extern int  API_PowerMDevGetWatchDog(); 
extern int  API_PowerMDevSetWatchDog(); 
extern int  API_PowerMDevWatchDogOff(); 
extern int  API_PowerMCpuGet(); 
extern int  API_PowerMCpuSet(); 
extern int  API_PowerMResume(); 
extern int  API_PowerMSavingEnter(); 
extern int  API_PowerMSavingExit(); 
extern int  API_PowerMSuspend(); 
extern int  alarm(); 
extern int  getitimer(); 
extern int  setitimer(); 
extern int  timer_create(); 
extern int  timer_create_internal(); 
extern int  timer_delete(); 
extern int  timer_getoverrun(); 
extern int  timer_getoverrun_64(); 
extern int  timer_gettime(); 
extern int  timer_setfile(); 
extern int  timer_settime(); 
extern int  ualarm(); 
extern int  _tmrfdCallback(); 
extern int  API_TimerfdDevCreate(); 
extern int  API_TimerfdDrvInstall(); 
extern int  timerfd_create(); 
extern int  timerfd_gettime(); 
extern int  timerfd_settime(); 
extern int  __selDoIoctls(); 
extern int  _SelectInit(); 
extern int  __selFdsetInit(); 
extern int  pselect(); 
extern int  select(); 
extern int  select_method(); 
extern int  API_SelWakeupListInit(); 
extern int  API_SelWakeupListLen(); 
extern int  API_SelWakeupListTerm(); 
extern int  API_SelNodeAdd(); 
extern int  API_SelNodeDelete(); 
extern int  API_SelWakeup(); 
extern int  API_SelWakeupAll(); 
extern int  API_SelWakeupAllByFlags(); 
extern int  API_SelWakeupError(); 
extern int  API_SelWakeupFifo(); 
extern int  API_SelWakeupPrio(); 
extern int  API_SelWakeupTerm(); 
extern int  API_SelWakeupType(); 
extern int  __selPtyAdd(); 
extern int  __selPtyDelete(); 
extern int  __selTyAdd(); 
extern int  __selTyDelete(); 
extern int  waitexcept(); 
extern int  waitread(); 
extern int  waitwrite(); 
extern int  bsd_signal(); 
extern int  kill(); 
extern int  pause(); 
extern int  raise(); 
extern int  sigaction(); 
extern int  sigaddset(); 
extern int  sigaltstack(); 
extern int  sigblock(); 
extern int  sigdelset(); 
extern int  sigemptyset(); 
extern int  sigfillset(); 
extern int  sigGetAction(); 
extern int  siggetmask(); 
extern int  sighold(); 
extern int  sigignore(); 
extern int  siginterrupt(); 
extern int  sigismember(); 
extern int  sigmask(); 
extern int  signal(); 
extern int  sigpause(); 
extern int  sigpending(); 
extern int  sigprocmask(); 
extern int  sigqueue(); 
extern int  sigrelse(); 
extern int  sigset(); 
extern int  sigsetmask(); 
extern int  sigstack(); 
extern int  sigsuspend(); 
extern int  sigtimedwait(); 
extern int  sigTrap(); 
extern int  sigvec(); 
extern int  sigwait(); 
extern int  sigwaitinfo(); 
extern int  _sigfdReadUnblock(); 
extern int  API_SignalfdDevCreate(); 
extern int  API_SignalfdDrvInstall(); 
extern int  signalfd(); 
extern int  __sigEventArgInit(); 
extern int  _doSigEvent(); 
extern int  _doSigEventEx(); 
extern int  __siglongjmpSetup(); 
extern int  __sigsetjmpSetup(); 
extern int  _doKill(); 
extern int  _doSignal(); 
extern int  _doSigQueue(); 
extern int  _sigGetAction(); 
extern int  _signalGetCtx(); 
extern int  _signalInit(); 
extern int  _sigPendFree(); 
extern int  _sigPendGet(); 
extern int  _sigPendInit(); 
extern int  _sigPendRun(); 
extern int  _sigTimeoutRecalc(); 
extern int  API_SystemHookAdd(); 
extern int  API_SystemHookDelete(); 
extern int  _HookListInit(); 
extern int  _SysInit(); 
extern int  API_ThreadPoolAddThread(); 
extern int  API_ThreadPoolCreate(); 
extern int  API_ThreadPoolDelete(); 
extern int  API_ThreadPoolDeleteThread(); 
extern int  API_ThreadPoolGetAttr(); 
extern int  API_ThreadPoolSetAttr(); 
extern int  API_ThreadPoolStatus(); 
extern int  _Allocate_ThreadPool_Object(); 
extern int  _Free_ThreadPool_Object(); 
extern int  _ThreadPoolInit(); 
extern int  _bmsgCreate(); 
extern int  _bmsgDelete(); 
extern int  _bmsgFlush(); 
extern int  _bmsgFreeByte(); 
extern int  _bmsgGet(); 
extern int  _bmsgIsEmpty(); 
extern int  _bmsgIsFull(); 
extern int  _bmsgNBytes(); 
extern int  _bmsgNBytesNext(); 
extern int  _bmsgPut(); 
extern int  _bmsgSizeGet(); 
extern int  _rngBufGet(); 
extern int  _rngBufPut(); 
extern int  _rngCreate(); 
extern int  _rngDelete(); 
extern int  _rngFlush(); 
extern int  _rngFreeBytes(); 
extern int  _rngIsEmpty(); 
extern int  _rngIsFull(); 
extern int  _rngMoveAhead(); 
extern int  _rngNBytes(); 
extern int  _rngPutAhead(); 
extern int  _rngSizeGet(); 
extern int  __aeabi_memclr(); 
extern int  __aeabi_memclr4(); 
extern int  __aeabi_memclr8(); 
extern int  __aeabi_memcpy(); 
extern int  __aeabi_memcpy4(); 
extern int  __aeabi_memcpy8(); 
extern int  __aeabi_memmove(); 
extern int  __aeabi_memmove4(); 
extern int  __aeabi_memmove8(); 
extern int  __aeabi_memset(); 
extern int  __aeabi_memset4(); 
extern int  __aeabi_memset8(); 
extern int  _ZdaPv(); 
extern int  _ZdlPv(); 
extern int  _Znaj(); 
extern int  _Znwj(); 
extern int  __cppRtDoCtors(); 
extern int  __cppRtDoDtors(); 
extern int  __cppRtDummy(); 
extern int  __cxa_atexit(); 
extern int  __cxa_finalize(); 
extern int  __cxa_guard_abort(); 
extern int  __cxa_guard_acquire(); 
extern int  __cxa_guard_release(); 
extern int  __cxa_module_finalize(); 
extern int  __cxa_pure_virtual(); 
extern int  _cppRtInit(); 
extern int  _cppRtUninit(); 
extern int  armAuxControlFeatureDisable(); 
extern int  armAuxControlFeatureEnable(); 
extern int  armControlFeatureDisable(); 
extern int  armControlFeatureEnable(); 
extern int  archIntHandle(); 
extern int  bspCpuExcHook(); 
extern int  archIntDisable(); 
extern int  archIntEnable(); 
extern int  archIntEnableForce(); 
extern int  armVfp11HwInit(); 
extern int  archMpCur(); 
extern int  archMpInt(); 
extern int  __diskCacheMemcpy(); 
extern int  _Exit(); 
extern int  lib_localeconv(); 
extern int  lib_setlocale(); 
extern int  init_etc_passwd_group(); 
extern int  init_etc_shadow(); 
extern int  _wcscasecmp(); 
extern int  _wcsdup(); 
extern int  _wcsncasecmp(); 
extern int  lwip_platform_memcpy(); 
extern int  lwip_platform_smemcpy(); 
extern int  API_PciDevInterDisonnect(); 
extern int  API_SysPerfInfo(); 
extern int  API_SysPerfRefresh(); 
extern int  API_SysPerfStart(); 
extern int  API_SysPerfStop(); 
extern int  __aeabi_atexit(); 
extern int  mbedtls_test_ca_crt; 
extern int  mbedtls_test_ca_key; 
extern int  mbedtls_test_ca_pwd; 
extern int  mbedtls_test_cas; 
extern int  mbedtls_test_cli_crt; 
extern int  mbedtls_test_cli_key; 
extern int  mbedtls_test_srv_crt; 
extern int  mbedtls_test_srv_key; 
extern int  mbedtls_mutex_free; 
extern int  mbedtls_mutex_init; 
extern int  mbedtls_mutex_lock; 
extern int  mbedtls_mutex_unlock; 
extern int  mbedtls_threading_gmtime_mutex; 
extern int  mbedtls_threading_readdir_mutex; 
extern int  cdio_version_string; 
extern int  yaffs_trace_mask; 
extern int  yaffsfs_deviceList; 
extern int  _G_cacheopLib; 
extern int  _K_timingKernel; 
extern int  _K_ulNCpus; 
extern int  _K_pfuncTimespecTimeoutTick; 
extern int  _K_pfuncTimespecTimeoutTick64; 
extern int  _ctype_; 
extern int  _tolower_tab_; 
extern int  _toupper_tab_; 
extern int  tzname; 
extern int  reachable_time; 
extern int  retrans_timer; 
extern int  csma_mac_driver; 
extern int  null_mac_driver; 
extern int  null_rdc_driver; 
extern int  tdma_mac_driver; 
extern int  snmp_community; 
extern int  snmp_community_trap; 
extern int  snmp_community_write; 
extern int  mq_attr_default; 
extern int  _G_stShellStackSize; 
extern int  _G_symStatic; 
extern int  _G_foGMemDrv; 
extern int  _G_iPipeDrvNum; 
extern int  _G_ulPipeLockOpt; 
extern int  _G_iSpipeDrvNum; 
extern int  _G_ulSpipeLockOpt; 
extern int  _G_iTycoDrvNum; 
extern int  _G_ulMutexOptionsTyLib; 
extern int  console_printk; 
extern int  mbedtls_timing_alarmed; 
extern int  _G_plineDiskCacheHeader; 
extern int  _G_ulDiskCacheListLock; 
extern int  _K_ulInterShowLock; 
extern int  _K_plineVmmVAddrSpaceHeader; 
extern int  _G_plineVProcHeader; 
extern int  _G_ulVProcMutex; 
extern int  nd6_queue_size; 
extern int  aodv_addr_any; 
extern int  _null_auth; 
extern int  _G_uiNamedNodeCounter; 
extern int  _G_ulPosixLock; 
extern int  _G_hTShellLock; 
extern int  _G_pfuncTSVarHook; 
extern int  _G_stSymbolCounter; 
extern int  _G_stSymbolNameTotalLen; 
extern int  _G_ulSymbolTableLock; 
extern int  _G_hPciCtrlHandle; 
extern int  _G_iPciVerbose; 
extern int  _G_bPowerSavingMode; 
extern int  mbedtls_test_ca_crt_ec; 
extern int  mbedtls_test_ca_crt_ec_len; 
extern int  mbedtls_test_ca_crt_len; 
extern int  mbedtls_test_ca_crt_rsa; 
extern int  mbedtls_test_ca_crt_rsa_len; 
extern int  mbedtls_test_ca_key_ec; 
extern int  mbedtls_test_ca_key_ec_len; 
extern int  mbedtls_test_ca_key_len; 
extern int  mbedtls_test_ca_key_rsa; 
extern int  mbedtls_test_ca_key_rsa_len; 
extern int  mbedtls_test_ca_pwd_ec; 
extern int  mbedtls_test_ca_pwd_ec_len; 
extern int  mbedtls_test_ca_pwd_len; 
extern int  mbedtls_test_ca_pwd_rsa; 
extern int  mbedtls_test_ca_pwd_rsa_len; 
extern int  mbedtls_test_cas_len; 
extern int  mbedtls_test_cas_pem; 
extern int  mbedtls_test_cas_pem_len; 
extern int  mbedtls_test_cli_crt_ec; 
extern int  mbedtls_test_cli_crt_ec_len; 
extern int  mbedtls_test_cli_crt_len; 
extern int  mbedtls_test_cli_crt_rsa; 
extern int  mbedtls_test_cli_crt_rsa_len; 
extern int  mbedtls_test_cli_key_ec; 
extern int  mbedtls_test_cli_key_ec_len; 
extern int  mbedtls_test_cli_key_len; 
extern int  mbedtls_test_cli_key_rsa; 
extern int  mbedtls_test_cli_key_rsa_len; 
extern int  mbedtls_test_srv_crt_ec; 
extern int  mbedtls_test_srv_crt_ec_len; 
extern int  mbedtls_test_srv_crt_len; 
extern int  mbedtls_test_srv_crt_rsa; 
extern int  mbedtls_test_srv_crt_rsa_len; 
extern int  mbedtls_test_srv_key_ec; 
extern int  mbedtls_test_srv_key_ec_len; 
extern int  mbedtls_test_srv_key_len; 
extern int  mbedtls_test_srv_key_rsa; 
extern int  mbedtls_test_srv_key_rsa_len; 
extern int  mbedtls_cipher_definitions; 
extern int  mbedtls_md2_info; 
extern int  mbedtls_md4_info; 
extern int  mbedtls_md5_info; 
extern int  mbedtls_ripemd160_info; 
extern int  mbedtls_sha1_info; 
extern int  mbedtls_sha224_info; 
extern int  mbedtls_sha256_info; 
extern int  mbedtls_sha384_info; 
extern int  mbedtls_sha512_info; 
extern int  mbedtls_ecdsa_info; 
extern int  mbedtls_eckey_info; 
extern int  mbedtls_eckeydh_info; 
extern int  mbedtls_rsa_alt_info; 
extern int  mbedtls_rsa_info; 
extern int  mbedtls_x509_crt_profile_default; 
extern int  mbedtls_x509_crt_profile_next; 
extern int  mbedtls_x509_crt_profile_suiteb; 
extern int  deflate_copyright; 
extern int  dictionary; 
extern int  hello; 
extern int  inflate_copyright; 
extern int  _dist_code; 
extern int  _length_code; 
extern int  z_errmsg; 
extern int  CDIO_SECTOR_SYNC_HEADER; 
extern int  libcdio_version_num; 
extern int  ISO_XA_MARKER_STRING; 
extern int  ISO_STANDARD_ID; 
extern int  nand_flash_ids; 
extern int  nand_manuf_ids; 
extern int  _K_cKernelLogo; 
extern int  _C_ctype_; 
extern int  _C_tolower_; 
extern int  _C_toupper_; 
extern int  _G_cMonth; 
extern int  _G_cWeek; 
extern int  in6addr_linklocal_allnodes; 
extern int  in6addr_loopback; 
extern int  in6addr_nodelocal_allnodes; 
extern int  ncrc_rev_table; 
extern int  crc16_table; 
extern int  netconn_aborted; 
extern int  netconn_closed; 
extern int  netconn_deleted; 
extern int  netconn_reset; 
extern int  dns_mquery_v4group; 
extern int  dns_mquery_v6group; 
extern int  ip_addr_any_type; 
extern int  memp_ARP_QUEUE; 
extern int  memp_FRAG_PBUF; 
extern int  memp_IGMP_GROUP; 
extern int  memp_IP6_REASSDATA; 
extern int  memp_LOCALHOSTLIST; 
extern int  memp_MLD6_GROUP; 
extern int  memp_ND6_QUEUE; 
extern int  memp_NETBUF; 
extern int  memp_NETCONN; 
extern int  memp_NETDB; 
extern int  memp_PBUF; 
extern int  memp_PBUF_POOL; 
extern int  memp_pools; 
extern int  memp_RAW_PCB; 
extern int  memp_REASSDATA; 
extern int  memp_SYS_TIMEOUT; 
extern int  memp_TCP_PCB; 
extern int  memp_TCP_PCB_LISTEN; 
extern int  memp_TCP_SEG; 
extern int  memp_TCPIP_MSG_API; 
extern int  memp_TCPIP_MSG_INPKT; 
extern int  memp_UDP_PCB; 
extern int  tcp_pcb_lists; 
extern int  lwip_cyclic_timers; 
extern int  lwip_num_cyclic_timers; 
extern int  ip_addr_any; 
extern int  ip_addr_broadcast; 
extern int  dhcp6_All_DHCP6_Relay_Agents_and_Servers; 
extern int  dhcp6_All_DHCP6_Servers; 
extern int  in6addr_any; 
extern int  ip6_addr_any; 
extern int  ethbroadcast; 
extern int  ethzero; 
extern int  md5_digest; 
extern int  chap_protent; 
extern int  chapms_digest; 
extern int  chapms2_digest; 
extern int  eap_protent; 
extern int  ipcp_protent; 
extern int  ipv6cp_protent; 
extern int  lcp_protent; 
extern int  memp_PPP_PCB; 
extern int  protocols; 
extern int  memp_PPPOE_IF; 
extern int  memp_PPPOL2TP_PCB; 
extern int  memp_PPPOS_PCB; 
extern int  pap_protent; 
extern int  snmp_zero_dot_zero; 
extern int  snmp_zero_dot_zero_values; 
extern int  mib2; 
extern int  snmp_mib2_icmp_root; 
extern int  snmp_mib2_interface_root; 
extern int  snmp_mib2_at_root; 
extern int  snmp_mib2_ip_root; 
extern int  snmp_mib2_snmp_root; 
extern int  snmp_mib2_system_node; 
extern int  snmp_mib2_tcp_root; 
extern int  snmp_mib2_udp_root; 
extern int  snmpframeworkmib; 
extern int  usmAESPrivProtocol; 
extern int  usmDESPrivProtocol; 
extern int  usmHMACMD5AuthProtocol; 
extern int  usmHMACSHAAuthProtocol; 
extern int  usmNoAuthProtocol; 
extern int  usmNoPrivProtocol; 
extern int  snmpusmmib; 
extern int  ptr_to_globals; 
extern int  mbedtls_cipher_supported; 
extern int  dictId; 
extern int  prog; 
extern int  _G_openocdInfo; 
extern int  iso_rock_enums; 
extern int  iso_rock_nm_flag; 
extern int  iso_rock_sl_flag; 
extern int  iso_rock_tf_flag; 
extern int  debugger_xa_misc_enum; 
extern int  iso_enums1; 
extern int  iso_extension_enums; 
extern int  iso_flag_enums; 
extern int  iso_vd_enums; 
extern int  mtd_table; 
extern int  _G_pfsrRoot; 
extern int  _G_ulProcFsLock; 
extern int  _G_devhdrRoot; 
extern int  _G_rfsrRoot; 
extern int  yaffs_wr_attempts; 
extern int  _K__partBuffer; 
extern int  _K_atomic64KernelTime; 
extern int  _K_bHeapCrossBorderEn; 
extern int  _K_cpuTable; 
extern int  _K_esBuffer; 
extern int  _K_eventBuffer; 
extern int  _K_heapBuffer; 
extern int  _K_hookKernel; 
extern int  _K_idescTable; 
extern int  _K_iTODDelta; 
extern int  _K_iTODDeltaNs; 
extern int  _K_jobmsgKernel; 
extern int  _K_jobqKernel; 
extern int  _K_klKernel; 
extern int  _K_msgqueueBuffer; 
extern int  _K_pcbbmapGlobalReady; 
extern int  _K_pfuncKernelDebugError; 
extern int  _K_pfuncKernelDebugLog; 
extern int  _K_pheapKernel; 
extern int  _K_pheapSystem; 
extern int  _K_plineTCBHeader; 
extern int  _K_ptcbIdle; 
extern int  _K_ptcbTCBIdTable; 
extern int  _K_resrcEvent; 
extern int  _K_resrcEventSet; 
extern int  _K_resrcHeap; 
extern int  _K_resrcMsgQueue; 
extern int  _K_resrcPart; 
extern int  _K_resrcRms; 
extern int  _K_resrcTcb; 
extern int  _K_resrcTmr; 
extern int  _K_rmsBuffer; 
extern int  _K_slcaAtomic; 
extern int  _K_slcaVectorTable; 
extern int  _K_slKernTime; 
extern int  _K_stkFreeFlag; 
extern int  _K_stkInterruptStack; 
extern int  _K_tcbBuffer; 
extern int  _K_tcbDummy; 
extern int  _K_tmrBuffer; 
extern int  _K_tvTODCurrent; 
extern int  _K_tvTODMono; 
extern int  _K_twjTable; 
extern int  _K_ucSysStatus; 
extern int  _K_ulCPUUsageEnable; 
extern int  _K_ulCPUUsageKernelTicks; 
extern int  _K_ulCPUUsageTicks; 
extern int  _K_ulIdleId; 
extern int  _K_ulKernFlags; 
extern int  _K_ulNotRunError; 
extern int  _K_ulThreadItimerId; 
extern int  _K_usThreadCounter; 
extern int  _K_wuDelay; 
extern int  _K_wuHTmr; 
extern int  _K_wuITmr; 
extern int  _K_wuWatchDog; 
extern int  _K_pfuncHeapTraceAlloc; 
extern int  _K_pfuncHeapTraceFree; 
extern int  _G_vmzonePhysical; 
extern int  _G_mmuOpLib; 
extern int  _G_ulVmmLock; 
extern int  __sdidinit; 
extern int  __sglue; 
extern int  timezone; 
extern int  _G_ulExecShareLock; 
extern int  _G_pvprocTable; 
extern int  _G_vprocKernel; 
extern int  _G_dpmaBuffer; 
extern int  _G_resrcDpma; 
extern int  _proto_stayopen; 
extern int  _serv_stayopen; 
extern int  _G_ulNetLibcLock; 
extern int  h_errno; 
extern int  lock_tcpip_core; 
extern int  ip_data; 
extern int  memp_memory_ARP_QUEUE_base; 
extern int  memp_memory_FRAG_PBUF_base; 
extern int  memp_memory_IGMP_GROUP_base; 
extern int  memp_memory_IP6_REASSDATA_base; 
extern int  memp_memory_LOCALHOSTLIST_base; 
extern int  memp_memory_MLD6_GROUP_base; 
extern int  memp_memory_ND6_QUEUE_base; 
extern int  memp_memory_NETBUF_base; 
extern int  memp_memory_NETCONN_base; 
extern int  memp_memory_NETDB_base; 
extern int  memp_memory_PBUF_base; 
extern int  memp_memory_PBUF_POOL_base; 
extern int  memp_memory_RAW_PCB_base; 
extern int  memp_memory_REASSDATA_base; 
extern int  memp_memory_SYS_TIMEOUT_base; 
extern int  memp_memory_TCP_PCB_base; 
extern int  memp_memory_TCP_PCB_LISTEN_base; 
extern int  memp_memory_TCP_SEG_base; 
extern int  memp_memory_TCPIP_MSG_API_base; 
extern int  memp_memory_TCPIP_MSG_INPKT_base; 
extern int  memp_memory_UDP_PCB_base; 
extern int  netif_default; 
extern int  netif_list; 
extern int  pbuf_free_ooseq_pending; 
extern int  raw_pcbs; 
extern int  lwip_stats; 
extern int  tcp_active_pcbs; 
extern int  tcp_active_pcbs_changed; 
extern int  tcp_bound_pcbs; 
extern int  tcp_listen_pcbs; 
extern int  tcp_ticks; 
extern int  tcp_tw_pcbs; 
extern int  tcp_input_pcb; 
extern int  udp_pcbs; 
extern int  dhcp_rx_options_given; 
extern int  dhcp_rx_options_val; 
extern int  dhcp6_rx_options; 
extern int  default_router_list; 
extern int  destination_cache; 
extern int  neighbor_cache; 
extern int  prefix_list; 
extern int  aodv_ipaddr_any; 
extern int  aodv_netif; 
extern int  aodv_host_state; 
extern int  aodv_rt_tbl; 
extern int  memp_memory_PPP_PCB_base; 
extern int  memp_memory_PPPOE_IF_base; 
extern int  memp_memory_PPPOL2TP_PCB_base; 
extern int  memp_memory_PPPOS_PCB_base; 
extern int  lowpan_addr_context_t; 
extern int  netbiosns_pcb; 
extern int  __key_decryptsession_pk_LOCAL; 
extern int  __key_encryptsession_pk_LOCAL; 
extern int  __key_gendes_LOCAL; 
extern int  rpc_createerr; 
extern int  svc_fdset; 
extern int  svc_max_pollfd; 
extern int  svc_pollfd; 
extern int  snmp_stats; 
extern int  snmp_mib2_lwip_locks; 
extern int  snmp_inform_callback; 
extern int  snmp_inform_callback_arg; 
extern int  snmp_write_callback; 
extern int  snmp_write_callback_arg; 
extern int  username; 
extern int  snmp_traps_handle; 
extern int  _G_aioqueue; 
extern int  _G_plinePxNameNodeHash; 
extern int  _G_plineSymbolHeaderHashTbl; 
extern int  _G_ulSpipeReadOpenLock; 
extern int  _G_ulSpipeWriteOpenLock; 
extern int  _S_atomicFileLineOp; 
extern int  _S_bFileEntryRemoveReq; 
extern int  _S_deventryTbl; 
extern int  _S_iIoMaxLinkLevels; 
extern int  _S_ioeIoGlobalEnv; 
extern int  _S_pfuncFileDescGet; 
extern int  _S_pfuncFileDup; 
extern int  _S_pfuncFileDup2; 
extern int  _S_pfuncFileGet; 
extern int  _S_pfuncFileRefDec; 
extern int  _S_pfuncFileRefGet; 
extern int  _S_pfuncFileRefInc; 
extern int  _S_pfuncGetCurPid; 
extern int  _S_plineDevHdrHeader; 
extern int  _S_plineFileEntryHeader; 
extern int  _S_resrcThreadPool; 
extern int  _S_threadpoolBuffer; 
extern int  _S_ulPowerMId; 
extern int  _S_ulThreadExceId; 
extern int  _S_ulThreadLogId; 
extern int  _G_hLogMsgHandle; 
extern int  _G_plinePMAdapter; 
extern int  _G_plinePMDev; 
extern int  _G_ulPowerMLock; 
extern int  _G_hookTable; 
extern int  __IO_BASE__; 
extern int  _impure_ptr; 
/*********************************************************************************************************	
  系统静态符号表										
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN										
    SYMBOL_ITEM_FUNC(bb_putchar) 
    SYMBOL_ITEM_FUNC(full_write) 
    SYMBOL_ITEM_FUNC(get_terminal_width_height) 
    SYMBOL_ITEM_FUNC(last_char_is) 
    SYMBOL_ITEM_FUNC(lib_xzalloc) 
    SYMBOL_ITEM_FUNC(vi_safe_poll) 
    SYMBOL_ITEM_FUNC(vi_safe_read) 
    SYMBOL_ITEM_FUNC(vi_safe_write) 
    SYMBOL_ITEM_FUNC(viShellInit) 
    SYMBOL_ITEM_FUNC(vi_main) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_crypt_cfb128) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_crypt_cfb8) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_crypt_ctr) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_free) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_init) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_setkey_dec) 
    SYMBOL_ITEM_FUNC(mbedtls_aes_setkey_enc) 
    SYMBOL_ITEM_FUNC(mbedtls_internal_aes_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_internal_aes_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_arc4_crypt) 
    SYMBOL_ITEM_FUNC(mbedtls_arc4_free) 
    SYMBOL_ITEM_FUNC(mbedtls_arc4_init) 
    SYMBOL_ITEM_FUNC(mbedtls_arc4_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_arc4_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_find_named_data) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_free_named_data) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_free_named_data_list) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_alg_null) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_bitstring) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_bitstring_null) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_bool) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_int) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_len) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_sequence_of) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_get_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_store_named_data) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_algorithm_identifier) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_bitstring) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_bool) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_ia5_string) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_int) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_len) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_null) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_octet_string) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_oid) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_printable_string) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_raw_buffer) 
    SYMBOL_ITEM_FUNC(mbedtls_asn1_write_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_base64_decode) 
    SYMBOL_ITEM_FUNC(mbedtls_base64_encode) 
    SYMBOL_ITEM_FUNC(mbedtls_base64_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_add_abs) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_add_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_add_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_bitlen) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_cmp_abs) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_cmp_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_cmp_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_copy) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_div_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_div_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_exp_mod) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_fill_random) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_free) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_gcd) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_gen_prime) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_get_bit) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_grow) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_init) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_inv_mod) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_is_prime) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_lsb) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_lset) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_mod_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_mod_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_mul_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_mul_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_read_binary) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_read_file) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_read_string) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_safe_cond_assign) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_safe_cond_swap) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_set_bit) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_shift_l) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_shift_r) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_shrink) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_size) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_sub_abs) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_sub_int) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_sub_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_swap) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_write_binary) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_write_file) 
    SYMBOL_ITEM_FUNC(mbedtls_mpi_write_string) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_crypt_cfb64) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_crypt_ctr) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_free) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_init) 
    SYMBOL_ITEM_FUNC(mbedtls_blowfish_setkey) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_crypt_cfb128) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_crypt_ctr) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_free) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_init) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_setkey_dec) 
    SYMBOL_ITEM_FUNC(mbedtls_camellia_setkey_enc) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_auth_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_encrypt_and_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_ccm_setkey) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_auth_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_auth_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_check_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_crypt) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_free) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_info_from_string) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_info_from_type) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_info_from_values) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_init) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_list) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_reset) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_set_iv) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_set_padding_mode) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_setkey) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_update) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_update_ad) 
    SYMBOL_ITEM_FUNC(mbedtls_cipher_write_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_random) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_random_with_add) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_reseed) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_seed) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_seed_entropy_len) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_set_entropy_len) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_set_prediction_resistance) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_set_reseed_interval) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_update) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_update_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_ctr_drbg_write_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_buf) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_crt) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_ecp) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_mpi) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_msg) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_print_ret) 
    SYMBOL_ITEM_FUNC(mbedtls_debug_set_threshold) 
    SYMBOL_ITEM_FUNC(mbedtls_des_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_des_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_des_free) 
    SYMBOL_ITEM_FUNC(mbedtls_des_init) 
    SYMBOL_ITEM_FUNC(mbedtls_des_key_check_key_parity) 
    SYMBOL_ITEM_FUNC(mbedtls_des_key_check_weak) 
    SYMBOL_ITEM_FUNC(mbedtls_des_key_set_parity) 
    SYMBOL_ITEM_FUNC(mbedtls_des_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_des_setkey) 
    SYMBOL_ITEM_FUNC(mbedtls_des_setkey_dec) 
    SYMBOL_ITEM_FUNC(mbedtls_des_setkey_enc) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_free) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_init) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_set2key_dec) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_set2key_enc) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_set3key_dec) 
    SYMBOL_ITEM_FUNC(mbedtls_des3_set3key_enc) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_calc_secret) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_free) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_init) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_make_params) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_make_public) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_parse_dhm) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_parse_dhmfile) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_read_params) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_read_public) 
    SYMBOL_ITEM_FUNC(mbedtls_dhm_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_calc_secret) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_compute_shared) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_gen_public) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_get_params) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_make_params) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_make_public) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_read_params) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdh_read_public) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_from_keypair) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_genkey) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_read_signature) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_sign) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_sign_det) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_write_signature) 
    SYMBOL_ITEM_FUNC(mbedtls_ecdsa_write_signature_det) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_group_load) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_check_privkey) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_check_pub_priv) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_check_pubkey) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_copy) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_curve_info_from_grp_id) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_curve_info_from_name) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_curve_info_from_tls_id) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_curve_list) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_gen_key) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_gen_keypair) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_gen_keypair_base) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_group_copy) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_group_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_group_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_grp_id_list) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_is_zero) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_keypair_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_keypair_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_mul) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_muladd) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_cmp) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_read_binary) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_read_string) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_point_write_binary) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_set_zero) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_tls_read_group) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_tls_read_point) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_tls_write_group) 
    SYMBOL_ITEM_FUNC(mbedtls_ecp_tls_write_point) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_add_source) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_free) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_func) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_gather) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_init) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_update_manual) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_update_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_entropy_write_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_hardclock_poll) 
    SYMBOL_ITEM_FUNC(mbedtls_platform_entropy_poll) 
    SYMBOL_ITEM_FUNC(mbedtls_strerror) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_auth_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_crypt_and_tag) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_free) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_init) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_setkey) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_gcm_update) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_free) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_init) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_random) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_random_with_add) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_reseed) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_seed) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_seed_buf) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_set_entropy_len) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_set_prediction_resistance) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_set_reseed_interval) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_update) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_update_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_hmac_drbg_write_seed_file) 
    SYMBOL_ITEM_FUNC(mbedtls_md) 
    SYMBOL_ITEM_FUNC(mbedtls_md_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_md_file) 
    SYMBOL_ITEM_FUNC(mbedtls_md_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_md_free) 
    SYMBOL_ITEM_FUNC(mbedtls_md_get_name) 
    SYMBOL_ITEM_FUNC(mbedtls_md_get_size) 
    SYMBOL_ITEM_FUNC(mbedtls_md_get_type) 
    SYMBOL_ITEM_FUNC(mbedtls_md_hmac) 
    SYMBOL_ITEM_FUNC(mbedtls_md_hmac_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_md_hmac_reset) 
    SYMBOL_ITEM_FUNC(mbedtls_md_hmac_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_md_hmac_update) 
    SYMBOL_ITEM_FUNC(mbedtls_md_info_from_string) 
    SYMBOL_ITEM_FUNC(mbedtls_md_info_from_type) 
    SYMBOL_ITEM_FUNC(mbedtls_md_init) 
    SYMBOL_ITEM_FUNC(mbedtls_md_init_ctx) 
    SYMBOL_ITEM_FUNC(mbedtls_md_list) 
    SYMBOL_ITEM_FUNC(mbedtls_md_process) 
    SYMBOL_ITEM_FUNC(mbedtls_md_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_md_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_md_update) 
    SYMBOL_ITEM_FUNC(mbedtls_md2) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_free) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_init) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_process) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_md2_update) 
    SYMBOL_ITEM_FUNC(mbedtls_md4) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_free) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_init) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_process) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_md4_update) 
    SYMBOL_ITEM_FUNC(mbedtls_md5) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_free) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_init) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_process) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_md5_update) 
    SYMBOL_ITEM_FUNC(mbedtls_net_accept) 
    SYMBOL_ITEM_FUNC(mbedtls_net_bind) 
    SYMBOL_ITEM_FUNC(mbedtls_net_connect) 
    SYMBOL_ITEM_FUNC(mbedtls_net_free) 
    SYMBOL_ITEM_FUNC(mbedtls_net_init) 
    SYMBOL_ITEM_FUNC(mbedtls_net_recv) 
    SYMBOL_ITEM_FUNC(mbedtls_net_recv_timeout) 
    SYMBOL_ITEM_FUNC(mbedtls_net_send) 
    SYMBOL_ITEM_FUNC(mbedtls_net_set_block) 
    SYMBOL_ITEM_FUNC(mbedtls_net_set_nonblock) 
    SYMBOL_ITEM_FUNC(mbedtls_net_usleep) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_attr_short_name) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_cipher_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_ec_grp) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_extended_key_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_md_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_numeric_string) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_oid_by_ec_grp) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_oid_by_md) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_oid_by_pk_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_oid_by_sig_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_pk_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_pkcs12_pbe_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_sig_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_sig_alg_desc) 
    SYMBOL_ITEM_FUNC(mbedtls_oid_get_x509_ext_type) 
    SYMBOL_ITEM_FUNC(mbedtls_pem_free) 
    SYMBOL_ITEM_FUNC(mbedtls_pem_init) 
    SYMBOL_ITEM_FUNC(mbedtls_pem_read_buffer) 
    SYMBOL_ITEM_FUNC(mbedtls_pem_write_buffer) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_can_do) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_check_pair) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_debug) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_free) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_get_bitlen) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_get_name) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_get_type) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_info_from_type) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_init) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_setup_rsa_alt) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_sign) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_verify_ext) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs12_derivation) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs12_pbe) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs12_pbe_sha1_rc4_128) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs5_pbes2) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs5_pbkdf2_hmac) 
    SYMBOL_ITEM_FUNC(mbedtls_pkcs5_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_load_file) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_parse_key) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_parse_keyfile) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_parse_public_key) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_parse_public_keyfile) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_parse_subpubkey) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_write_key_der) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_write_key_pem) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_write_pubkey) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_write_pubkey_der) 
    SYMBOL_ITEM_FUNC(mbedtls_pk_write_pubkey_pem) 
    SYMBOL_ITEM_FUNC(mbedtls_platform_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_platform_teardown) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_process) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_ripemd160_update) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_check_privkey) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_check_pub_priv) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_check_pubkey) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_copy) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_free) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_gen_key) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_init) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_pkcs1_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_pkcs1_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_pkcs1_sign) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_pkcs1_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_private) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_public) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsaes_oaep_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsaes_oaep_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsaes_pkcs1_v15_decrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsaes_pkcs1_v15_encrypt) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsassa_pkcs1_v15_sign) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsassa_pkcs1_v15_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsassa_pss_sign) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsassa_pss_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_rsassa_pss_verify_ext) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_rsa_set_padding) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_free) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_init) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_process) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_sha1_update) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_free) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_init) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_process) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_sha256_update) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_clone) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_finish) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_free) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_init) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_process) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_starts) 
    SYMBOL_ITEM_FUNC(mbedtls_sha512_update) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_get) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_set) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_set_max_entries) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cache_set_timeout) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ciphersuite_from_id) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ciphersuite_from_string) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ciphersuite_uses_ec) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ciphersuite_uses_psk) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_ciphersuite_id) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_ciphersuite_name) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_ciphersuite_sig_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_ciphersuite_sig_pk_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_list_ciphersuites) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake_client_step) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_check) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_set_timeout) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_cookie_write) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dtls_cookies) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake_server_step) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_client_transport_id) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ticket_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ticket_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ticket_parse) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ticket_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_ticket_write) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_check_cert_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_check_curve) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_check_sig_hash) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_close_notify) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_alpn_protocols) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_arc4_support) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_authmode) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_ca_chain) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_cbc_record_splitting) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_cert_profile) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_cert_req_ca_list) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_ciphersuites) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_ciphersuites_for_version) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_curves) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dbg) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dh_param) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dh_param_ctx) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dhm_min_bitlen) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dtls_anti_replay) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_dtls_badmac_limit) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_encrypt_then_mac) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_endpoint) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_export_keys_cb) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_extended_master_secret) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_fallback) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_handshake_timeout) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_legacy_renegotiation) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_max_frag_len) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_max_version) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_min_version) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_own_cert) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_psk) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_psk_cb) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_read_timeout) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_renegotiation) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_renegotiation_enforced) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_renegotiation_period) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_rng) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_session_cache) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_session_tickets) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_session_tickets_cb) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_sig_hashes) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_sni) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_transport) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_truncated_hmac) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_conf_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_config_defaults) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_config_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_config_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_derive_keys) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_dtls_replay_check) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_dtls_replay_update) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_fetch_input) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_flush_output) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_alpn_protocol) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_bio) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_bytes_avail) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_ciphersuite) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_max_frag_len) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_peer_cert) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_record_expansion) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_session) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_verify_result) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_get_version) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handle_message_type) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake_step) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_handshake_wrapup) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_hash_from_md_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_md_alg_from_hash) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_optimize_checksum) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_parse_certificate) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_parse_change_cipher_spec) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_parse_finished) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_pk_alg_from_sig) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_prepare_handshake_record) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_psk_derive_premaster) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_read) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_read_record) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_read_record_layer) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_read_version) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_recv_flight_completed) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_renegotiate) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_resend) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_reset_checksum) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_send_alert_message) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_send_fatal_handshake_failure) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_send_flight_completed) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_session_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_session_init) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_session_reset) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_bio) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_calc_verify_md) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_hostname) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_hs_authmode) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_hs_ca_chain) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_hs_own_cert) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_hs_psk) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_session) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_set_timer_cb) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_setup) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_sig_from_pk) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_sig_from_pk_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_sig_hash_set_add) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_sig_hash_set_const_hash) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_sig_hash_set_find) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_transform_free) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_update_handshake_status) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write_certificate) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write_change_cipher_spec) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write_finished) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write_record) 
    SYMBOL_ITEM_FUNC(mbedtls_ssl_write_version) 
    SYMBOL_ITEM_FUNC(mbedtls_set_alarm) 
    SYMBOL_ITEM_FUNC(mbedtls_timing_get_delay) 
    SYMBOL_ITEM_FUNC(mbedtls_timing_get_timer) 
    SYMBOL_ITEM_FUNC(mbedtls_timing_hardclock) 
    SYMBOL_ITEM_FUNC(mbedtls_timing_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_timing_set_delay) 
    SYMBOL_ITEM_FUNC(mbedtls_version_get_number) 
    SYMBOL_ITEM_FUNC(mbedtls_version_get_string) 
    SYMBOL_ITEM_FUNC(mbedtls_version_get_string_full) 
    SYMBOL_ITEM_FUNC(mbedtls_version_check_feature) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_dn_gets) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_alg_null) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_ext) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_name) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_rsassa_pss_params) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_serial) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_sig) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_sig_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_get_time) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_key_size_helper) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_serial_gets) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_sig_alg_gets) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_time_is_future) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_time_is_past) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_der) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_free) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_init) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_pem) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_authority_key_identifier) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_basic_constraints) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_extension) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_issuer_key) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_issuer_name) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_key_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_md_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_ns_cert_type) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_serial) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_subject_key) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_subject_key_identifier) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_subject_name) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_validity) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_crt_set_version) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_der) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_free) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_init) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_pem) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_extension) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_key) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_key_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_md_alg) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_ns_cert_type) 
    SYMBOL_ITEM_FUNC(mbedtls_x509write_csr_set_subject_name) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_set_extension) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_string_to_names) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_write_extensions) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_write_names) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_write_sig) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_free) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_info) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_init) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_parse) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_parse_der) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crl_parse_file) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_check_extended_key_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_check_key_usage) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_free) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_info) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_init) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_is_revoked) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_parse) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_parse_der) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_parse_file) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_parse_path) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_verify) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_verify_info) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_crt_verify_with_profile) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_free) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_info) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_init) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_parse) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_parse_der) 
    SYMBOL_ITEM_FUNC(mbedtls_x509_csr_parse_file) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_crypt_cbc) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_crypt_ecb) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_free) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_init) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_self_test) 
    SYMBOL_ITEM_FUNC(mbedtls_xtea_setup) 
    SYMBOL_ITEM_FUNC(zlibShellInit) 
    SYMBOL_ITEM_FUNC(adler32) 
    SYMBOL_ITEM_FUNC(adler32_combine) 
    SYMBOL_ITEM_FUNC(adler32_combine64) 
    SYMBOL_ITEM_FUNC(compress) 
    SYMBOL_ITEM_FUNC(compress2) 
    SYMBOL_ITEM_FUNC(compressBound) 
    SYMBOL_ITEM_FUNC(crc32) 
    SYMBOL_ITEM_FUNC(crc32_combine) 
    SYMBOL_ITEM_FUNC(crc32_combine64) 
    SYMBOL_ITEM_FUNC(get_crc_table) 
    SYMBOL_ITEM_FUNC(deflate) 
    SYMBOL_ITEM_FUNC(deflateBound) 
    SYMBOL_ITEM_FUNC(deflateCopy) 
    SYMBOL_ITEM_FUNC(deflateEnd) 
    SYMBOL_ITEM_FUNC(deflateInit_) 
    SYMBOL_ITEM_FUNC(deflateInit2_) 
    SYMBOL_ITEM_FUNC(deflateParams) 
    SYMBOL_ITEM_FUNC(deflatePending) 
    SYMBOL_ITEM_FUNC(deflatePrime) 
    SYMBOL_ITEM_FUNC(deflateReset) 
    SYMBOL_ITEM_FUNC(deflateResetKeep) 
    SYMBOL_ITEM_FUNC(deflateSetDictionary) 
    SYMBOL_ITEM_FUNC(deflateSetHeader) 
    SYMBOL_ITEM_FUNC(deflateTune) 
    SYMBOL_ITEM_FUNC(test_compress) 
    SYMBOL_ITEM_FUNC(test_deflate) 
    SYMBOL_ITEM_FUNC(test_dict_deflate) 
    SYMBOL_ITEM_FUNC(test_dict_inflate) 
    SYMBOL_ITEM_FUNC(test_flush) 
    SYMBOL_ITEM_FUNC(test_gzio) 
    SYMBOL_ITEM_FUNC(test_inflate) 
    SYMBOL_ITEM_FUNC(test_large_deflate) 
    SYMBOL_ITEM_FUNC(test_large_inflate) 
    SYMBOL_ITEM_FUNC(test_sync) 
    SYMBOL_ITEM_FUNC(zlib_main) 
    SYMBOL_ITEM_FUNC(gzclose) 
    SYMBOL_ITEM_FUNC(gz_error) 
    SYMBOL_ITEM_FUNC(gzbuffer) 
    SYMBOL_ITEM_FUNC(gzclearerr) 
    SYMBOL_ITEM_FUNC(gzdopen) 
    SYMBOL_ITEM_FUNC(gzeof) 
    SYMBOL_ITEM_FUNC(gzerror) 
    SYMBOL_ITEM_FUNC(gzoffset) 
    SYMBOL_ITEM_FUNC(gzoffset64) 
    SYMBOL_ITEM_FUNC(gzopen) 
    SYMBOL_ITEM_FUNC(gzopen64) 
    SYMBOL_ITEM_FUNC(gzrewind) 
    SYMBOL_ITEM_FUNC(gzseek) 
    SYMBOL_ITEM_FUNC(gzseek64) 
    SYMBOL_ITEM_FUNC(gztell) 
    SYMBOL_ITEM_FUNC(gztell64) 
    SYMBOL_ITEM_FUNC(gzclose_r) 
    SYMBOL_ITEM_FUNC(gzdirect) 
    SYMBOL_ITEM_FUNC(gzgetc) 
    SYMBOL_ITEM_FUNC(gzgetc_) 
    SYMBOL_ITEM_FUNC(gzgets) 
    SYMBOL_ITEM_FUNC(gzread) 
    SYMBOL_ITEM_FUNC(gzungetc) 
    SYMBOL_ITEM_FUNC(gzclose_w) 
    SYMBOL_ITEM_FUNC(gzflush) 
    SYMBOL_ITEM_FUNC(gzprintf) 
    SYMBOL_ITEM_FUNC(gzputc) 
    SYMBOL_ITEM_FUNC(gzputs) 
    SYMBOL_ITEM_FUNC(gzsetparams) 
    SYMBOL_ITEM_FUNC(gzvprintf) 
    SYMBOL_ITEM_FUNC(gzwrite) 
    SYMBOL_ITEM_FUNC(inflateBack) 
    SYMBOL_ITEM_FUNC(inflateBackEnd) 
    SYMBOL_ITEM_FUNC(inflateBackInit_) 
    SYMBOL_ITEM_FUNC(inflate_fast) 
    SYMBOL_ITEM_FUNC(inflate) 
    SYMBOL_ITEM_FUNC(inflateCopy) 
    SYMBOL_ITEM_FUNC(inflateEnd) 
    SYMBOL_ITEM_FUNC(inflateGetDictionary) 
    SYMBOL_ITEM_FUNC(inflateGetHeader) 
    SYMBOL_ITEM_FUNC(inflateInit_) 
    SYMBOL_ITEM_FUNC(inflateInit2_) 
    SYMBOL_ITEM_FUNC(inflateMark) 
    SYMBOL_ITEM_FUNC(inflatePrime) 
    SYMBOL_ITEM_FUNC(inflateReset) 
    SYMBOL_ITEM_FUNC(inflateReset2) 
    SYMBOL_ITEM_FUNC(inflateResetKeep) 
    SYMBOL_ITEM_FUNC(inflateSetDictionary) 
    SYMBOL_ITEM_FUNC(inflateSync) 
    SYMBOL_ITEM_FUNC(inflateSyncPoint) 
    SYMBOL_ITEM_FUNC(inflateUndermine) 
    SYMBOL_ITEM_FUNC(inflate_table) 
    SYMBOL_ITEM_FUNC(file_compress) 
    SYMBOL_ITEM_FUNC(file_uncompress) 
    SYMBOL_ITEM_FUNC(gz_compress) 
    SYMBOL_ITEM_FUNC(gz_uncompress) 
    SYMBOL_ITEM_FUNC(minigzip_main) 
    SYMBOL_ITEM_FUNC(_tr_align) 
    SYMBOL_ITEM_FUNC(_tr_flush_bits) 
    SYMBOL_ITEM_FUNC(_tr_flush_block) 
    SYMBOL_ITEM_FUNC(_tr_init) 
    SYMBOL_ITEM_FUNC(_tr_stored_block) 
    SYMBOL_ITEM_FUNC(_tr_tally) 
    SYMBOL_ITEM_FUNC(uncompress) 
    SYMBOL_ITEM_FUNC(zcalloc) 
    SYMBOL_ITEM_FUNC(zcfree) 
    SYMBOL_ITEM_FUNC(zError) 
    SYMBOL_ITEM_FUNC(zlibCompileFlags) 
    SYMBOL_ITEM_FUNC(zlibVersion) 
    SYMBOL_ITEM_FUNC(backtrace) 
    SYMBOL_ITEM_FUNC(armA1xL2CtlGet) 
    SYMBOL_ITEM_FUNC(armA1xL2CtlSet) 
    SYMBOL_ITEM_FUNC(armAsyncBusMode) 
    SYMBOL_ITEM_FUNC(armBranchPredictionDisable) 
    SYMBOL_ITEM_FUNC(armBranchPredictionEnable) 
    SYMBOL_ITEM_FUNC(armBranchPredictorInvalidate) 
    SYMBOL_ITEM_FUNC(armCp15AuxCtrlReg) 
    SYMBOL_ITEM_FUNC(armCp15ControlReg) 
    SYMBOL_ITEM_FUNC(armCp15LtbTypeReg) 
    SYMBOL_ITEM_FUNC(armCp15MainIdReg) 
    SYMBOL_ITEM_FUNC(armCp15TcmTypeReg) 
    SYMBOL_ITEM_FUNC(armFastBusMode) 
    SYMBOL_ITEM_FUNC(armHighVectorDisable) 
    SYMBOL_ITEM_FUNC(armHighVectorEnable) 
    SYMBOL_ITEM_FUNC(armPrivatePeriphBaseGet) 
    SYMBOL_ITEM_FUNC(armSyncBusMode) 
    SYMBOL_ITEM_FUNC(armVectorBaseAddrSet) 
    SYMBOL_ITEM_FUNC(armWaitForInterrupt) 
    SYMBOL_ITEM_FUNC(archAssert) 
    SYMBOL_ITEM_FUNC(archCtxStackEnd) 
    SYMBOL_ITEM_FUNC(archIntCtxSaveReg) 
    SYMBOL_ITEM_FUNC(archTaskCtxCreate) 
    SYMBOL_ITEM_FUNC(archTaskCtxPrint) 
    SYMBOL_ITEM_FUNC(archTaskCtxSetFp) 
    SYMBOL_ITEM_FUNC(archTaskCtxShow) 
    SYMBOL_ITEM_FUNC(archTaskRegsGet) 
    SYMBOL_ITEM_FUNC(archTaskRegsSet) 
    SYMBOL_ITEM_FUNC(archCrtCtxSwitch) 
    SYMBOL_ITEM_FUNC(archIntCtxLoad) 
    SYMBOL_ITEM_FUNC(archSigCtxLoad) 
    SYMBOL_ITEM_FUNC(archTaskCtxCopy) 
    SYMBOL_ITEM_FUNC(archTaskCtxStart) 
    SYMBOL_ITEM_FUNC(archTaskCtxSwitch) 
    SYMBOL_ITEM_FUNC(archAbtHandle) 
    SYMBOL_ITEM_FUNC(archSwiHandle) 
    SYMBOL_ITEM_FUNC(archUndHandle) 
    SYMBOL_ITEM_FUNC(archAbtEntry) 
    SYMBOL_ITEM_FUNC(archIntEntry) 
    SYMBOL_ITEM_FUNC(archPreEntry) 
    SYMBOL_ITEM_FUNC(archSwiEntry) 
    SYMBOL_ITEM_FUNC(archUndEntry) 
    SYMBOL_ITEM_FUNC(archFindLsb) 
    SYMBOL_ITEM_FUNC(archFindMsb) 
    SYMBOL_ITEM_FUNC(archBogoMipsLoop) 
    SYMBOL_ITEM_FUNC(archGetCpsr) 
    SYMBOL_ITEM_FUNC(archPageCopy) 
    SYMBOL_ITEM_FUNC(archReboot) 
    SYMBOL_ITEM_FUNC(longjmp) 
    SYMBOL_ITEM_FUNC(setjmp) 
    SYMBOL_ITEM_FUNC(siglongjmp) 
    SYMBOL_ITEM_FUNC(sigsetjmp) 
    SYMBOL_ITEM_FUNC(archDbgAbInsert) 
    SYMBOL_ITEM_FUNC(archDbgApRemove) 
    SYMBOL_ITEM_FUNC(archDbgBpAdjust) 
    SYMBOL_ITEM_FUNC(archDbgBpInsert) 
    SYMBOL_ITEM_FUNC(archDbgBpPrefetch) 
    SYMBOL_ITEM_FUNC(archDbgBpRemove) 
    SYMBOL_ITEM_FUNC(archDbgTrapType) 
    SYMBOL_ITEM_FUNC(archGdbCoreXml) 
    SYMBOL_ITEM_FUNC(archGdbGetNextPc) 
    SYMBOL_ITEM_FUNC(archGdbGetStepSkip) 
    SYMBOL_ITEM_FUNC(archGdbRegGetPc) 
    SYMBOL_ITEM_FUNC(archGdbRegSetPc) 
    SYMBOL_ITEM_FUNC(archGdbRegsGet) 
    SYMBOL_ITEM_FUNC(archGdbRegsSet) 
    SYMBOL_ITEM_FUNC(archGdbTargetXml) 
    SYMBOL_ITEM_FUNC(armGetNextPc) 
    SYMBOL_ITEM_FUNC(armDmaPl330Add) 
    SYMBOL_ITEM_FUNC(armDmaPl330GetFuncs) 
    SYMBOL_ITEM_FUNC(armDmaPl330Isr) 
    SYMBOL_ITEM_FUNC(archElfGotInit) 
    SYMBOL_ITEM_FUNC(archElfRelocateRel) 
    SYMBOL_ITEM_FUNC(archElfRelocateRela) 
    SYMBOL_ITEM_FUNC(archElfRGetJmpBuffItemLen) 
    SYMBOL_ITEM_FUNC(dl_unwind_find_exidx) 
    SYMBOL_ITEM_FUNC(armVfp9PrimaryInit) 
    SYMBOL_ITEM_FUNC(armVfp9SecondaryInit) 
    SYMBOL_ITEM_FUNC(armVfp9Disable) 
    SYMBOL_ITEM_FUNC(armVfp9Enable) 
    SYMBOL_ITEM_FUNC(armVfp9IsEnable) 
    SYMBOL_ITEM_FUNC(armVfp9Restore16) 
    SYMBOL_ITEM_FUNC(armVfp9Restore32) 
    SYMBOL_ITEM_FUNC(armVfp9Save16) 
    SYMBOL_ITEM_FUNC(armVfp9Save32) 
    SYMBOL_ITEM_FUNC(armVfp9Sid) 
    SYMBOL_ITEM_FUNC(armVfp11PrimaryInit) 
    SYMBOL_ITEM_FUNC(armVfp11SecondaryInit) 
    SYMBOL_ITEM_FUNC(armVfp11Mvfr0) 
    SYMBOL_ITEM_FUNC(armVfp11NonSecEn) 
    SYMBOL_ITEM_FUNC(armVfpV3PrimaryInit) 
    SYMBOL_ITEM_FUNC(armVfpV3SecondaryInit) 
    SYMBOL_ITEM_FUNC(armVfpV3Restore16) 
    SYMBOL_ITEM_FUNC(armVfpV3Restore32) 
    SYMBOL_ITEM_FUNC(armVfpV3Save16) 
    SYMBOL_ITEM_FUNC(armVfpV3Save32) 
    SYMBOL_ITEM_FUNC(armVfpV4PrimaryInit) 
    SYMBOL_ITEM_FUNC(armVfpV4SecondaryInit) 
    SYMBOL_ITEM_FUNC(armVfpNonePrimaryInit) 
    SYMBOL_ITEM_FUNC(armVfpNoneSecondaryInit) 
    SYMBOL_ITEM_FUNC(archFpuCtxInit) 
    SYMBOL_ITEM_FUNC(archFpuCtxShow) 
    SYMBOL_ITEM_FUNC(archFpuDisable) 
    SYMBOL_ITEM_FUNC(archFpuEnable) 
    SYMBOL_ITEM_FUNC(archFpuPrimaryInit) 
    SYMBOL_ITEM_FUNC(archFpuRestore) 
    SYMBOL_ITEM_FUNC(archFpuSave) 
    SYMBOL_ITEM_FUNC(archFpuSecondaryInit) 
    SYMBOL_ITEM_FUNC(archFpuUndHandle) 
    SYMBOL_ITEM_FUNC(armL2Clear) 
    SYMBOL_ITEM_FUNC(armL2ClearAll) 
    SYMBOL_ITEM_FUNC(armL2Disable) 
    SYMBOL_ITEM_FUNC(armL2Enable) 
    SYMBOL_ITEM_FUNC(armL2Flush) 
    SYMBOL_ITEM_FUNC(armL2FlushAll) 
    SYMBOL_ITEM_FUNC(armL2Init) 
    SYMBOL_ITEM_FUNC(armL2Invalidate) 
    SYMBOL_ITEM_FUNC(armL2InvalidateAll) 
    SYMBOL_ITEM_FUNC(armL2IsEnable) 
    SYMBOL_ITEM_FUNC(armL2Name) 
    SYMBOL_ITEM_FUNC(armL2Sync) 
    SYMBOL_ITEM_FUNC(armL2A17Init) 
    SYMBOL_ITEM_FUNC(armL2A8Init) 
    SYMBOL_ITEM_FUNC(armL2x0Init) 
    SYMBOL_ITEM_FUNC(armCacheV4Init) 
    SYMBOL_ITEM_FUNC(armCacheV4Reset) 
    SYMBOL_ITEM_FUNC(armDCacheV4ClearAll) 
    SYMBOL_ITEM_FUNC(armDCacheV4Disable) 
    SYMBOL_ITEM_FUNC(armDCacheV4FlushAll) 
    SYMBOL_ITEM_FUNC(armDCacheV4InvalidateAll) 
    SYMBOL_ITEM_FUNC(armCacheV5Init) 
    SYMBOL_ITEM_FUNC(armCacheV5Reset) 
    SYMBOL_ITEM_FUNC(armDCacheV5ClearAll) 
    SYMBOL_ITEM_FUNC(armDCacheV5Disable) 
    SYMBOL_ITEM_FUNC(armDCacheV5FlushAll) 
    SYMBOL_ITEM_FUNC(armDCacheV5InvalidateAll) 
    SYMBOL_ITEM_FUNC(armCacheV6Init) 
    SYMBOL_ITEM_FUNC(armCacheV6Reset) 
    SYMBOL_ITEM_FUNC(armDCacheV6ClearAll) 
    SYMBOL_ITEM_FUNC(armDCacheV6Disable) 
    SYMBOL_ITEM_FUNC(armDCacheV6FlushAll) 
    SYMBOL_ITEM_FUNC(armDCacheV6InvalidateAll) 
    SYMBOL_ITEM_FUNC(armCacheV7Init) 
    SYMBOL_ITEM_FUNC(armCacheV7Reset) 
    SYMBOL_ITEM_FUNC(armCacheV7CCSIDR) 
    SYMBOL_ITEM_FUNC(armCacheV7CLIDR) 
    SYMBOL_ITEM_FUNC(armCacheV7CSSELR) 
    SYMBOL_ITEM_FUNC(armCacheV7CTR) 
    SYMBOL_ITEM_FUNC(armCacheV7SetCSSELR) 
    SYMBOL_ITEM_FUNC(armDCacheV7ClearAll) 
    SYMBOL_ITEM_FUNC(armDCacheV7ClearAllPoU) 
    SYMBOL_ITEM_FUNC(armDCacheV7Disable) 
    SYMBOL_ITEM_FUNC(armDCacheV7FlushAll) 
    SYMBOL_ITEM_FUNC(armDCacheV7FlushAllPoU) 
    SYMBOL_ITEM_FUNC(armDCacheV7FlushPoU) 
    SYMBOL_ITEM_FUNC(armDCacheV7InvalidateAll) 
    SYMBOL_ITEM_FUNC(armDCacheV7InvalidateAllPoU) 
    SYMBOL_ITEM_FUNC(armCacheV8Init) 
    SYMBOL_ITEM_FUNC(armCacheV8Reset) 
    SYMBOL_ITEM_FUNC(armCacheRetireDefault) 
    SYMBOL_ITEM_FUNC(armCacheRetireRR) 
    SYMBOL_ITEM_FUNC(armCacheTypeReg) 
    SYMBOL_ITEM_FUNC(armDCacheClear) 
    SYMBOL_ITEM_FUNC(armDCacheEnable) 
    SYMBOL_ITEM_FUNC(armDCacheFlush) 
    SYMBOL_ITEM_FUNC(armDCacheInvalidate) 
    SYMBOL_ITEM_FUNC(armICacheDisable) 
    SYMBOL_ITEM_FUNC(armICacheEnable) 
    SYMBOL_ITEM_FUNC(armICacheInvalidate) 
    SYMBOL_ITEM_FUNC(armICacheInvalidateAll) 
    SYMBOL_ITEM_FUNC(armMmuV4Init) 
    SYMBOL_ITEM_FUNC(armMmuV7ForceDevType) 
    SYMBOL_ITEM_FUNC(armMmuV7ForceNonSecure) 
    SYMBOL_ITEM_FUNC(armMmuV7ForceShare) 
    SYMBOL_ITEM_FUNC(armMmuV7Init) 
    SYMBOL_ITEM_FUNC(armMmuV7GetTTBCR) 
    SYMBOL_ITEM_FUNC(armMmuV7SetTTBCR) 
    SYMBOL_ITEM_FUNC(armGetAbtAddr) 
    SYMBOL_ITEM_FUNC(armGetAbtType) 
    SYMBOL_ITEM_FUNC(armGetPreAddr) 
    SYMBOL_ITEM_FUNC(armGetPreType) 
    SYMBOL_ITEM_FUNC(armMmuAbtFaultAddr) 
    SYMBOL_ITEM_FUNC(armMmuAbtFaultStatus) 
    SYMBOL_ITEM_FUNC(armMmuDisable) 
    SYMBOL_ITEM_FUNC(armMmuDisableAlignFault) 
    SYMBOL_ITEM_FUNC(armMmuDisableWriteBuffer) 
    SYMBOL_ITEM_FUNC(armMmuEnable) 
    SYMBOL_ITEM_FUNC(armMmuEnableAlignFault) 
    SYMBOL_ITEM_FUNC(armMmuEnableWriteBuffer) 
    SYMBOL_ITEM_FUNC(armMmuInitSysRom) 
    SYMBOL_ITEM_FUNC(armMmuInvalidateTLB) 
    SYMBOL_ITEM_FUNC(armMmuInvalidateTLBMVA) 
    SYMBOL_ITEM_FUNC(armMmuPreFaultStatus) 
    SYMBOL_ITEM_FUNC(armMmuSetDomain) 
    SYMBOL_ITEM_FUNC(armMmuSetProcessId) 
    SYMBOL_ITEM_FUNC(armMmuSetTTBase) 
    SYMBOL_ITEM_FUNC(armMmuSetTTBase1) 
    SYMBOL_ITEM_FUNC(archCacheInit) 
    SYMBOL_ITEM_FUNC(archCacheReset) 
    SYMBOL_ITEM_FUNC(archMmuInit) 
    SYMBOL_ITEM_FUNC(armScuAccessCtrlSet) 
    SYMBOL_ITEM_FUNC(armScuCpuMpStatus) 
    SYMBOL_ITEM_FUNC(armScuCpuNumber) 
    SYMBOL_ITEM_FUNC(armScuCpuPowerStatusGet) 
    SYMBOL_ITEM_FUNC(armScuFeatureDisable) 
    SYMBOL_ITEM_FUNC(armScuFeatureEnable) 
    SYMBOL_ITEM_FUNC(armScuFeatureGet) 
    SYMBOL_ITEM_FUNC(armScuFilteringSet) 
    SYMBOL_ITEM_FUNC(armScuNonAccessCtrlSet) 
    SYMBOL_ITEM_FUNC(armScuSecureInvalidateAll) 
    SYMBOL_ITEM_FUNC(armScuTagRamSize) 
    SYMBOL_ITEM_FUNC(archSpinBypass) 
    SYMBOL_ITEM_FUNC(archSpinDelay) 
    SYMBOL_ITEM_FUNC(archSpinInit) 
    SYMBOL_ITEM_FUNC(archSpinLock) 
    SYMBOL_ITEM_FUNC(archSpinLockRaw) 
    SYMBOL_ITEM_FUNC(archSpinNotify) 
    SYMBOL_ITEM_FUNC(archSpinTryLock) 
    SYMBOL_ITEM_FUNC(archSpinTryLockRaw) 
    SYMBOL_ITEM_FUNC(archSpinUnlock) 
    SYMBOL_ITEM_FUNC(archSpinUnlockRaw) 
    SYMBOL_ITEM_FUNC(archSpinWork) 
    SYMBOL_ITEM_FUNC(archKernelParam) 
    SYMBOL_ITEM_FUNC(archKernelParamGet) 
    SYMBOL_ITEM_FUNC(API_DtraceAbortTrap) 
    SYMBOL_ITEM_FUNC(API_DtraceBreakpointInsert) 
    SYMBOL_ITEM_FUNC(API_DtraceBreakpointRemove) 
    SYMBOL_ITEM_FUNC(API_DtraceBreakTrap) 
    SYMBOL_ITEM_FUNC(API_DtraceChildSig) 
    SYMBOL_ITEM_FUNC(API_DtraceContinueProcess) 
    SYMBOL_ITEM_FUNC(API_DtraceContinueThread) 
    SYMBOL_ITEM_FUNC(API_DtraceCreate) 
    SYMBOL_ITEM_FUNC(API_DtraceDelBreakInfo) 
    SYMBOL_ITEM_FUNC(API_DtraceDelete) 
    SYMBOL_ITEM_FUNC(API_DtraceGetBreakInfo) 
    SYMBOL_ITEM_FUNC(API_DtraceGetFpuRegs) 
    SYMBOL_ITEM_FUNC(API_DtraceGetMems) 
    SYMBOL_ITEM_FUNC(API_DtraceGetRegs) 
    SYMBOL_ITEM_FUNC(API_DtraceIsValid) 
    SYMBOL_ITEM_FUNC(API_DtraceProcessThread) 
    SYMBOL_ITEM_FUNC(API_DtraceSchedHook) 
    SYMBOL_ITEM_FUNC(API_DtraceSetFpuRegs) 
    SYMBOL_ITEM_FUNC(API_DtraceSetMems) 
    SYMBOL_ITEM_FUNC(API_DtraceSetPid) 
    SYMBOL_ITEM_FUNC(API_DtraceSetRegs) 
    SYMBOL_ITEM_FUNC(API_DtraceStopProcess) 
    SYMBOL_ITEM_FUNC(API_DtraceStopThread) 
    SYMBOL_ITEM_FUNC(API_DtraceThreadExtraInfo) 
    SYMBOL_ITEM_FUNC(API_DtraceThreadStepGet) 
    SYMBOL_ITEM_FUNC(API_DtraceThreadStepSet) 
    SYMBOL_ITEM_FUNC(API_DtraceWatchpointInsert) 
    SYMBOL_ITEM_FUNC(API_DtraceWatchpointRemove) 
    SYMBOL_ITEM_FUNC(API_GdbInit) 
    SYMBOL_ITEM_FUNC(API_GdbModuleInit) 
    SYMBOL_ITEM_FUNC(API_OpenOCDStep1) 
    SYMBOL_ITEM_FUNC(API_OpenOCDStep2) 
    SYMBOL_ITEM_FUNC(API_OpenOCDStep3) 
    SYMBOL_ITEM_FUNC(sja1000Init) 
    SYMBOL_ITEM_FUNC(sja1000Isr) 
    SYMBOL_ITEM_FUNC(i8237aGetFuncs) 
    SYMBOL_ITEM_FUNC(i8259aInit) 
    SYMBOL_ITEM_FUNC(i8259aIrq) 
    SYMBOL_ITEM_FUNC(i8259aIrqDisable) 
    SYMBOL_ITEM_FUNC(i8259aIrqEnable) 
    SYMBOL_ITEM_FUNC(i8259aIrqEoi) 
    SYMBOL_ITEM_FUNC(i8259aIrqIsEnable) 
    SYMBOL_ITEM_FUNC(i8259aIrqIsPending) 
    SYMBOL_ITEM_FUNC(dm9000Init) 
    SYMBOL_ITEM_FUNC(dm9000Isr) 
    SYMBOL_ITEM_FUNC(smethndInit) 
    SYMBOL_ITEM_FUNC(smethndNotify) 
    SYMBOL_ITEM_FUNC(pciSioExarChan) 
    SYMBOL_ITEM_FUNC(pciSioExarInit) 
    SYMBOL_ITEM_FUNC(pciSioNetmosInit) 
    SYMBOL_ITEM_FUNC(pciStorageAtaInit) 
    SYMBOL_ITEM_FUNC(pciStorageNvmeInit) 
    SYMBOL_ITEM_FUNC(pciStorageSataInit) 
    SYMBOL_ITEM_FUNC(sio16c550Init) 
    SYMBOL_ITEM_FUNC(sio16c550Isr) 
    SYMBOL_ITEM_FUNC(sio16c550RxIsr) 
    SYMBOL_ITEM_FUNC(sio16c550TxIsr) 
    SYMBOL_ITEM_FUNC(i8254BuzzerOff) 
    SYMBOL_ITEM_FUNC(i8254BuzzerOn) 
    SYMBOL_ITEM_FUNC(i8254GetCnt) 
    SYMBOL_ITEM_FUNC(i8254Init) 
    SYMBOL_ITEM_FUNC(i8254InitAsHtimer) 
    SYMBOL_ITEM_FUNC(i8254InitAsTick) 
    SYMBOL_ITEM_FUNC(API_DiskCacheCreate) 
    SYMBOL_ITEM_FUNC(API_DiskCacheCreateEx) 
    SYMBOL_ITEM_FUNC(API_DiskCacheCreateEx2) 
    SYMBOL_ITEM_FUNC(API_DiskCacheDelete) 
    SYMBOL_ITEM_FUNC(API_DiskCacheSync) 
    SYMBOL_ITEM_FUNC(__diskCacheFlushInvCnt) 
    SYMBOL_ITEM_FUNC(__diskCacheFlushInvMeta) 
    SYMBOL_ITEM_FUNC(__diskCacheFlushInvRange) 
    SYMBOL_ITEM_FUNC(__diskCacheFlushList) 
    SYMBOL_ITEM_FUNC(__diskCacheFsCallback) 
    SYMBOL_ITEM_FUNC(__diskCacheHashAdd) 
    SYMBOL_ITEM_FUNC(__diskCacheHashFind) 
    SYMBOL_ITEM_FUNC(__diskCacheHashRemove) 
    SYMBOL_ITEM_FUNC(__diskCacheIoctl) 
    SYMBOL_ITEM_FUNC(__diskCacheMemInit) 
    SYMBOL_ITEM_FUNC(__diskCacheMemReset) 
    SYMBOL_ITEM_FUNC(__diskCacheNodeAlloc) 
    SYMBOL_ITEM_FUNC(__diskCacheNodeFind) 
    SYMBOL_ITEM_FUNC(__diskCacheNodeGet) 
    SYMBOL_ITEM_FUNC(__diskCacheNodeRead) 
    SYMBOL_ITEM_FUNC(__diskCacheRead) 
    SYMBOL_ITEM_FUNC(__diskCacheReset) 
    SYMBOL_ITEM_FUNC(__diskCacheSortListAdd) 
    SYMBOL_ITEM_FUNC(__diskCacheStatusChk) 
    SYMBOL_ITEM_FUNC(__diskCacheWrite) 
    SYMBOL_ITEM_FUNC(__diskCacheWpCreate) 
    SYMBOL_ITEM_FUNC(__diskCacheWpDelete) 
    SYMBOL_ITEM_FUNC(__diskCacheWpGetBuffer) 
    SYMBOL_ITEM_FUNC(__diskCacheWpPutBuffer) 
    SYMBOL_ITEM_FUNC(__diskCacheWpRead) 
    SYMBOL_ITEM_FUNC(__diskCacheWpSteal) 
    SYMBOL_ITEM_FUNC(__diskCacheWpSync) 
    SYMBOL_ITEM_FUNC(__diskCacheWpWrite) 
    SYMBOL_ITEM_FUNC(__procFsDiskCacheInit) 
    SYMBOL_ITEM_FUNC(__diskCacheListAdd) 
    SYMBOL_ITEM_FUNC(__diskCacheListDel) 
    SYMBOL_ITEM_FUNC(__diskCacheSync) 
    SYMBOL_ITEM_FUNC(API_DiskPartitionFree) 
    SYMBOL_ITEM_FUNC(API_DiskPartitionGet) 
    SYMBOL_ITEM_FUNC(API_DiskPartitionLinkNumGet) 
    SYMBOL_ITEM_FUNC(API_DiskPartitionScan) 
    SYMBOL_ITEM_FUNC(API_DiskRiad0Create) 
    SYMBOL_ITEM_FUNC(API_DiskRiad0Delete) 
    SYMBOL_ITEM_FUNC(API_DiskRiad1Create) 
    SYMBOL_ITEM_FUNC(API_DiskRiad1Delete) 
    SYMBOL_ITEM_FUNC(API_DiskRiad1Ghost) 
    SYMBOL_ITEM_FUNC(__diskRaidBytesPerBlock) 
    SYMBOL_ITEM_FUNC(__diskRaidBytesPerSector) 
    SYMBOL_ITEM_FUNC(__diskRaidCheck) 
    SYMBOL_ITEM_FUNC(__diskRaidTotalSector) 
    SYMBOL_ITEM_FUNC(disk_initialize) 
    SYMBOL_ITEM_FUNC(disk_ioctl) 
    SYMBOL_ITEM_FUNC(disk_read) 
    SYMBOL_ITEM_FUNC(disk_status) 
    SYMBOL_ITEM_FUNC(disk_write) 
    SYMBOL_ITEM_FUNC(__fatFsGetError) 
    SYMBOL_ITEM_FUNC(API_FatFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_FatFsDevDelete) 
    SYMBOL_ITEM_FUNC(API_FatFsDrvInstall) 
    SYMBOL_ITEM_FUNC(__filInfoToStat) 
    SYMBOL_ITEM_FUNC(__fsAttrToMode) 
    SYMBOL_ITEM_FUNC(__fsInfoToStatfs) 
    SYMBOL_ITEM_FUNC(__fsModeToAttr) 
    SYMBOL_ITEM_FUNC(__fattimeToTime) 
    SYMBOL_ITEM_FUNC(__timeToFatTime) 
    SYMBOL_ITEM_FUNC(get_fattime) 
    SYMBOL_ITEM_FUNC(f_chmod) 
    SYMBOL_ITEM_FUNC(f_chmod_ex) 
    SYMBOL_ITEM_FUNC(f_close) 
    SYMBOL_ITEM_FUNC(f_closedir) 
    SYMBOL_ITEM_FUNC(f_expand) 
    SYMBOL_ITEM_FUNC(f_getfree) 
    SYMBOL_ITEM_FUNC(f_getlabel) 
    SYMBOL_ITEM_FUNC(f_getlabel_ex) 
    SYMBOL_ITEM_FUNC(f_lseek) 
    SYMBOL_ITEM_FUNC(f_mkdir) 
    SYMBOL_ITEM_FUNC(f_mkdir_ex) 
    SYMBOL_ITEM_FUNC(f_mkfs) 
    SYMBOL_ITEM_FUNC(f_mount) 
    SYMBOL_ITEM_FUNC(f_mount_ex) 
    SYMBOL_ITEM_FUNC(f_open) 
    SYMBOL_ITEM_FUNC(f_open_ex) 
    SYMBOL_ITEM_FUNC(f_opendir) 
    SYMBOL_ITEM_FUNC(f_opendir_ex) 
    SYMBOL_ITEM_FUNC(f_read) 
    SYMBOL_ITEM_FUNC(f_readdir) 
    SYMBOL_ITEM_FUNC(f_rename) 
    SYMBOL_ITEM_FUNC(f_rename_ex) 
    SYMBOL_ITEM_FUNC(f_setlabel) 
    SYMBOL_ITEM_FUNC(f_setlabel_ex) 
    SYMBOL_ITEM_FUNC(f_stat) 
    SYMBOL_ITEM_FUNC(f_stat_ex) 
    SYMBOL_ITEM_FUNC(f_sync) 
    SYMBOL_ITEM_FUNC(f_syncfs) 
    SYMBOL_ITEM_FUNC(f_truncate) 
    SYMBOL_ITEM_FUNC(f_unlink) 
    SYMBOL_ITEM_FUNC(f_unlink_ex) 
    SYMBOL_ITEM_FUNC(f_utime) 
    SYMBOL_ITEM_FUNC(f_utime_ex) 
    SYMBOL_ITEM_FUNC(f_write) 
    SYMBOL_ITEM_FUNC(ff_convert) 
    SYMBOL_ITEM_FUNC(ff_wtoupper) 
    SYMBOL_ITEM_FUNC(__fsCheckFileName) 
    SYMBOL_ITEM_FUNC(__fsCreateFuncGet) 
    SYMBOL_ITEM_FUNC(__fsDiskLinkCounterAdd) 
    SYMBOL_ITEM_FUNC(__fsDiskLinkCounterDec) 
    SYMBOL_ITEM_FUNC(__fsDiskLinkCounterGet) 
    SYMBOL_ITEM_FUNC(__fsPartitionProb) 
    SYMBOL_ITEM_FUNC(__fsRegister) 
    SYMBOL_ITEM_FUNC(_cdio_list_append) 
    SYMBOL_ITEM_FUNC(_cdio_list_begin) 
    SYMBOL_ITEM_FUNC(_cdio_list_end) 
    SYMBOL_ITEM_FUNC(_cdio_list_find) 
    SYMBOL_ITEM_FUNC(_cdio_list_foreach) 
    SYMBOL_ITEM_FUNC(_cdio_list_free) 
    SYMBOL_ITEM_FUNC(_cdio_list_length) 
    SYMBOL_ITEM_FUNC(_cdio_list_new) 
    SYMBOL_ITEM_FUNC(_cdio_list_node_data) 
    SYMBOL_ITEM_FUNC(_cdio_list_node_free) 
    SYMBOL_ITEM_FUNC(_cdio_list_node_next) 
    SYMBOL_ITEM_FUNC(_cdio_list_prepend) 
    SYMBOL_ITEM_FUNC(iso9660_close) 
    SYMBOL_ITEM_FUNC(iso9660_dir_to_name) 
    SYMBOL_ITEM_FUNC(iso9660_dirlist_free) 
    SYMBOL_ITEM_FUNC(iso9660_dirlist_new) 
    SYMBOL_ITEM_FUNC(iso9660_filelist_free) 
    SYMBOL_ITEM_FUNC(iso9660_filelist_new) 
    SYMBOL_ITEM_FUNC(iso9660_have_rr) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_find_lsn) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_find_lsn_with_path) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_fuzzy_read_superblock) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_application_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_joliet_level) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_preparer_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_publisher_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_system_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_volume_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_get_volumeset_id) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_is_xa) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_read_pvd) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_read_superblock) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_readdir) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_stat) 
    SYMBOL_ITEM_FUNC(iso9660_ifs_stat_translate) 
    SYMBOL_ITEM_FUNC(iso9660_iso_seek_read) 
    SYMBOL_ITEM_FUNC(iso9660_open) 
    SYMBOL_ITEM_FUNC(iso9660_open_ext) 
    SYMBOL_ITEM_FUNC(iso9660_open_fuzzy) 
    SYMBOL_ITEM_FUNC(iso9660_open_fuzzy_ext) 
    SYMBOL_ITEM_FUNC(iso9660_stat_free) 
    SYMBOL_ITEM_FUNC(cdio_calloc) 
    SYMBOL_ITEM_FUNC(cdio_free) 
    SYMBOL_ITEM_FUNC(cdio_stdio_destroy) 
    SYMBOL_ITEM_FUNC(cdio_stdio_new) 
    SYMBOL_ITEM_FUNC(cdio_stream_read) 
    SYMBOL_ITEM_FUNC(cdio_stream_seek) 
    SYMBOL_ITEM_FUNC(get_rock_ridge_filename) 
    SYMBOL_ITEM_FUNC(iso9660_get_posix_filemode_from_rock) 
    SYMBOL_ITEM_FUNC(iso9660_get_rock_attr_str) 
    SYMBOL_ITEM_FUNC(parse_rock_ridge_stat) 
    SYMBOL_ITEM_FUNC(cdio_lba_to_lsn) 
    SYMBOL_ITEM_FUNC(cdio_lba_to_msf) 
    SYMBOL_ITEM_FUNC(cdio_lba_to_msf_str) 
    SYMBOL_ITEM_FUNC(cdio_lsn_to_lba) 
    SYMBOL_ITEM_FUNC(cdio_lsn_to_msf) 
    SYMBOL_ITEM_FUNC(cdio_mmssff_to_lba) 
    SYMBOL_ITEM_FUNC(cdio_msf_to_lba) 
    SYMBOL_ITEM_FUNC(cdio_msf_to_lsn) 
    SYMBOL_ITEM_FUNC(cdio_msf_to_str) 
    SYMBOL_ITEM_FUNC(cdio_msf3_to_lba) 
    SYMBOL_ITEM_FUNC(API_Iso9660FsDevCreate) 
    SYMBOL_ITEM_FUNC(API_Iso9660FsDevDelete) 
    SYMBOL_ITEM_FUNC(API_Iso9660FsDrvInstall) 
    SYMBOL_ITEM_FUNC(cdio_charset_converter_create) 
    SYMBOL_ITEM_FUNC(cdio_charset_converter_destroy) 
    SYMBOL_ITEM_FUNC(cdio_charset_from_utf8) 
    SYMBOL_ITEM_FUNC(cdio_charset_to_utf8) 
    SYMBOL_ITEM_FUNC(_cdio_memdup) 
    SYMBOL_ITEM_FUNC(_cdio_strdup_fixpath) 
    SYMBOL_ITEM_FUNC(_cdio_strdup_upper) 
    SYMBOL_ITEM_FUNC(_cdio_strfreev) 
    SYMBOL_ITEM_FUNC(_cdio_strlenv) 
    SYMBOL_ITEM_FUNC(_cdio_strsplit) 
    SYMBOL_ITEM_FUNC(cdio_from_bcd8) 
    SYMBOL_ITEM_FUNC(cdio_to_bcd8) 
    SYMBOL_ITEM_FUNC(iso9660_get_posix_filemode_from_xa) 
    SYMBOL_ITEM_FUNC(iso9660_get_xa_attr_str) 
    SYMBOL_ITEM_FUNC(iso9660_xa_free) 
    SYMBOL_ITEM_FUNC(iso9660_xa_init) 
    SYMBOL_ITEM_FUNC(iso9660_dir_add_entry_su) 
    SYMBOL_ITEM_FUNC(iso9660_dir_calc_record_size) 
    SYMBOL_ITEM_FUNC(iso9660_dir_init_new) 
    SYMBOL_ITEM_FUNC(iso9660_dir_init_new_su) 
    SYMBOL_ITEM_FUNC(iso9660_dirname_valid_p) 
    SYMBOL_ITEM_FUNC(iso9660_get_application_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_dir_len) 
    SYMBOL_ITEM_FUNC(iso9660_get_dtime) 
    SYMBOL_ITEM_FUNC(iso9660_get_ltime) 
    SYMBOL_ITEM_FUNC(iso9660_get_posix_filemode) 
    SYMBOL_ITEM_FUNC(iso9660_get_preparer_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_publisher_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_pvd_block_size) 
    SYMBOL_ITEM_FUNC(iso9660_get_pvd_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_pvd_space_size) 
    SYMBOL_ITEM_FUNC(iso9660_get_pvd_type) 
    SYMBOL_ITEM_FUNC(iso9660_get_pvd_version) 
    SYMBOL_ITEM_FUNC(iso9660_get_root_lsn) 
    SYMBOL_ITEM_FUNC(iso9660_get_system_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_volume_id) 
    SYMBOL_ITEM_FUNC(iso9660_get_volumeset_id) 
    SYMBOL_ITEM_FUNC(iso9660_is_achar) 
    SYMBOL_ITEM_FUNC(iso9660_is_dchar) 
    SYMBOL_ITEM_FUNC(iso9660_name_translate) 
    SYMBOL_ITEM_FUNC(iso9660_name_translate_ext) 
    SYMBOL_ITEM_FUNC(iso9660_pathname_isofy) 
    SYMBOL_ITEM_FUNC(iso9660_pathname_valid_p) 
    SYMBOL_ITEM_FUNC(iso9660_pathtable_get_size) 
    SYMBOL_ITEM_FUNC(iso9660_pathtable_init) 
    SYMBOL_ITEM_FUNC(iso9660_pathtable_l_add_entry) 
    SYMBOL_ITEM_FUNC(iso9660_pathtable_m_add_entry) 
    SYMBOL_ITEM_FUNC(iso9660_set_dtime) 
    SYMBOL_ITEM_FUNC(iso9660_set_dtime_with_timezone) 
    SYMBOL_ITEM_FUNC(iso9660_set_evd) 
    SYMBOL_ITEM_FUNC(iso9660_set_ltime) 
    SYMBOL_ITEM_FUNC(iso9660_set_ltime_with_timezone) 
    SYMBOL_ITEM_FUNC(iso9660_set_pvd) 
    SYMBOL_ITEM_FUNC(iso9660_strncpy_pad) 
    SYMBOL_ITEM_FUNC(API_Mount) 
    SYMBOL_ITEM_FUNC(API_MountEx) 
    SYMBOL_ITEM_FUNC(API_MountInit) 
    SYMBOL_ITEM_FUNC(API_MountShow) 
    SYMBOL_ITEM_FUNC(API_Unmount) 
    SYMBOL_ITEM_FUNC(mount) 
    SYMBOL_ITEM_FUNC(umount) 
    SYMBOL_ITEM_FUNC(umount2) 
    SYMBOL_ITEM_FUNC(add_mtd_device) 
    SYMBOL_ITEM_FUNC(del_mtd_device) 
    SYMBOL_ITEM_FUNC(get_mtd_device) 
    SYMBOL_ITEM_FUNC(get_mtd_device_nm) 
    SYMBOL_ITEM_FUNC(mtd_block_isbad) 
    SYMBOL_ITEM_FUNC(mtd_block_markbad) 
    SYMBOL_ITEM_FUNC(mtd_erase) 
    SYMBOL_ITEM_FUNC(mtd_get_fact_prot_info) 
    SYMBOL_ITEM_FUNC(mtd_get_user_prot_info) 
    SYMBOL_ITEM_FUNC(mtd_lock) 
    SYMBOL_ITEM_FUNC(mtd_lock_user_prot_reg) 
    SYMBOL_ITEM_FUNC(mtd_panic_write) 
    SYMBOL_ITEM_FUNC(mtd_read) 
    SYMBOL_ITEM_FUNC(mtd_read_fact_prot_reg) 
    SYMBOL_ITEM_FUNC(mtd_read_oob) 
    SYMBOL_ITEM_FUNC(mtd_read_user_prot_reg) 
    SYMBOL_ITEM_FUNC(mtd_unlock) 
    SYMBOL_ITEM_FUNC(mtd_write) 
    SYMBOL_ITEM_FUNC(mtd_write_user_prot_reg) 
    SYMBOL_ITEM_FUNC(put_mtd_device) 
    SYMBOL_ITEM_FUNC(decode_bch) 
    SYMBOL_ITEM_FUNC(encode_bch) 
    SYMBOL_ITEM_FUNC(free_bch) 
    SYMBOL_ITEM_FUNC(init_bch) 
    SYMBOL_ITEM_FUNC(skip_spaces) 
    SYMBOL_ITEM_FUNC(strim) 
    SYMBOL_ITEM_FUNC(nand_erase_nand) 
    SYMBOL_ITEM_FUNC(nand_read_buf) 
    SYMBOL_ITEM_FUNC(nand_read_buf16) 
    SYMBOL_ITEM_FUNC(nand_read_byte) 
    SYMBOL_ITEM_FUNC(nand_release) 
    SYMBOL_ITEM_FUNC(nand_scan) 
    SYMBOL_ITEM_FUNC(nand_scan_ident) 
    SYMBOL_ITEM_FUNC(nand_scan_tail) 
    SYMBOL_ITEM_FUNC(nand_wait_ready) 
    SYMBOL_ITEM_FUNC(nand_write_buf) 
    SYMBOL_ITEM_FUNC(nand_write_buf16) 
    SYMBOL_ITEM_FUNC(memchr_inv) 
    SYMBOL_ITEM_FUNC(nand_default_bbt) 
    SYMBOL_ITEM_FUNC(nand_isbad_bbt) 
    SYMBOL_ITEM_FUNC(nand_scan_bbt) 
    SYMBOL_ITEM_FUNC(nand_update_bbt) 
    SYMBOL_ITEM_FUNC(nand_bch_calculate_ecc) 
    SYMBOL_ITEM_FUNC(nand_bch_correct_data) 
    SYMBOL_ITEM_FUNC(nand_bch_free) 
    SYMBOL_ITEM_FUNC(nand_bch_init) 
    SYMBOL_ITEM_FUNC(nand_calculate_ecc) 
    SYMBOL_ITEM_FUNC(nand_correct_data) 
    SYMBOL_ITEM_FUNC(flexonenand_region) 
    SYMBOL_ITEM_FUNC(flexonenand_set_boundary) 
    SYMBOL_ITEM_FUNC(onenand_addr) 
    SYMBOL_ITEM_FUNC(onenand_bbt_read_oob) 
    SYMBOL_ITEM_FUNC(onenand_block) 
    SYMBOL_ITEM_FUNC(onenand_block_isbad) 
    SYMBOL_ITEM_FUNC(onenand_block_markbad) 
    SYMBOL_ITEM_FUNC(onenand_erase) 
    SYMBOL_ITEM_FUNC(onenand_print_device_info) 
    SYMBOL_ITEM_FUNC(onenand_probe) 
    SYMBOL_ITEM_FUNC(onenand_read) 
    SYMBOL_ITEM_FUNC(onenand_read_oob) 
    SYMBOL_ITEM_FUNC(onenand_release) 
    SYMBOL_ITEM_FUNC(onenand_scan) 
    SYMBOL_ITEM_FUNC(onenand_sync) 
    SYMBOL_ITEM_FUNC(onenand_write) 
    SYMBOL_ITEM_FUNC(onenand_write_oob) 
    SYMBOL_ITEM_FUNC(onenand_default_bbt) 
    SYMBOL_ITEM_FUNC(onenand_scan_bbt) 
    SYMBOL_ITEM_FUNC(API_NandRCacheBlockFree) 
    SYMBOL_ITEM_FUNC(API_NandRCacheCreate) 
    SYMBOL_ITEM_FUNC(API_NandRCacheDelete) 
    SYMBOL_ITEM_FUNC(API_NandRCacheNodeAlloc) 
    SYMBOL_ITEM_FUNC(API_NandRCacheNodeFree) 
    SYMBOL_ITEM_FUNC(API_NandRCacheNodeGet) 
    SYMBOL_ITEM_FUNC(mountproc3_dump_3) 
    SYMBOL_ITEM_FUNC(mountproc3_export_3) 
    SYMBOL_ITEM_FUNC(mountproc3_mnt_3) 
    SYMBOL_ITEM_FUNC(mountproc3_null_3) 
    SYMBOL_ITEM_FUNC(mountproc3_umnt_3) 
    SYMBOL_ITEM_FUNC(mountproc3_umntall_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_access_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_commit_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_create_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_fsinfo_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_fsstat_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_getattr_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_link_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_lookup_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_mkdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_mknod_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_null_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_pathconf_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_read_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readdirplus_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readlink_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_remove_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_rename_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_rmdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_setattr_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_symlink_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_write_3) 
    SYMBOL_ITEM_FUNC(API_NfsDevCreate) 
    SYMBOL_ITEM_FUNC(API_NfsDevDelete) 
    SYMBOL_ITEM_FUNC(API_NfsDrvInstall) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3args) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3res) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3resfail) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3resok) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3args) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3res) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3resfail) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3resok) 
    SYMBOL_ITEM_FUNC(xdr_cookie3) 
    SYMBOL_ITEM_FUNC(xdr_cookieverf3) 
    SYMBOL_ITEM_FUNC(xdr_count3) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3args) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3res) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3resok) 
    SYMBOL_ITEM_FUNC(xdr_createhow3) 
    SYMBOL_ITEM_FUNC(xdr_createmode3) 
    SYMBOL_ITEM_FUNC(xdr_createverf3) 
    SYMBOL_ITEM_FUNC(xdr_devicedata3) 
    SYMBOL_ITEM_FUNC(xdr_dirlist3) 
    SYMBOL_ITEM_FUNC(xdr_dirlistplus3) 
    SYMBOL_ITEM_FUNC(xdr_diropargs3) 
    SYMBOL_ITEM_FUNC(xdr_dirpath) 
    SYMBOL_ITEM_FUNC(xdr_entry3) 
    SYMBOL_ITEM_FUNC(xdr_entryplus3) 
    SYMBOL_ITEM_FUNC(xdr_exportnode) 
    SYMBOL_ITEM_FUNC(xdr_exports) 
    SYMBOL_ITEM_FUNC(xdr_fattr3) 
    SYMBOL_ITEM_FUNC(xdr_fhandle3) 
    SYMBOL_ITEM_FUNC(xdr_fileid3) 
    SYMBOL_ITEM_FUNC(xdr_filename) 
    SYMBOL_ITEM_FUNC(xdr_filename3) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3args) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3res) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3resfail) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3resok) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3args) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3res) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3resfail) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3resok) 
    SYMBOL_ITEM_FUNC(xdr_ftype3) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3args) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3res) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3resok) 
    SYMBOL_ITEM_FUNC(xdr_gid3) 
    SYMBOL_ITEM_FUNC(xdr_groupnode) 
    SYMBOL_ITEM_FUNC(xdr_groups) 
    SYMBOL_ITEM_FUNC(xdr_int32) 
    SYMBOL_ITEM_FUNC(xdr_LINK3args) 
    SYMBOL_ITEM_FUNC(xdr_LINK3res) 
    SYMBOL_ITEM_FUNC(xdr_LINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_LINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3args) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3res) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3resfail) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3resok) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3args) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3res) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3resfail) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3resok) 
    SYMBOL_ITEM_FUNC(xdr_mknoddata3) 
    SYMBOL_ITEM_FUNC(xdr_mode3) 
    SYMBOL_ITEM_FUNC(xdr_mountbody) 
    SYMBOL_ITEM_FUNC(xdr_mountlist) 
    SYMBOL_ITEM_FUNC(xdr_mountres3) 
    SYMBOL_ITEM_FUNC(xdr_mountres3_ok) 
    SYMBOL_ITEM_FUNC(xdr_mountstat3) 
    SYMBOL_ITEM_FUNC(xdr_name) 
    SYMBOL_ITEM_FUNC(xdr_nfs_fh3) 
    SYMBOL_ITEM_FUNC(xdr_nfspath) 
    SYMBOL_ITEM_FUNC(xdr_nfspath3) 
    SYMBOL_ITEM_FUNC(xdr_nfsstat3) 
    SYMBOL_ITEM_FUNC(xdr_nfstime3) 
    SYMBOL_ITEM_FUNC(xdr_offset3) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3args) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3res) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3resfail) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3resok) 
    SYMBOL_ITEM_FUNC(xdr_post_op_attr) 
    SYMBOL_ITEM_FUNC(xdr_post_op_fh3) 
    SYMBOL_ITEM_FUNC(xdr_pre_op_attr) 
    SYMBOL_ITEM_FUNC(xdr_READ3args) 
    SYMBOL_ITEM_FUNC(xdr_READ3res) 
    SYMBOL_ITEM_FUNC(xdr_READ3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READ3resok) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3args) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3res) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3resok) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3args) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3res) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3args) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3res) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3resok) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3args) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3res) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3resfail) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3resok) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_sattr3) 
    SYMBOL_ITEM_FUNC(xdr_sattrguard3) 
    SYMBOL_ITEM_FUNC(xdr_set_atime) 
    SYMBOL_ITEM_FUNC(xdr_set_gid3) 
    SYMBOL_ITEM_FUNC(xdr_set_mode3) 
    SYMBOL_ITEM_FUNC(xdr_set_mtime) 
    SYMBOL_ITEM_FUNC(xdr_set_size3) 
    SYMBOL_ITEM_FUNC(xdr_set_uid3) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3args) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3res) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3resok) 
    SYMBOL_ITEM_FUNC(xdr_size3) 
    SYMBOL_ITEM_FUNC(xdr_specdata3) 
    SYMBOL_ITEM_FUNC(xdr_stable_how) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3args) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3res) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_symlinkdata3) 
    SYMBOL_ITEM_FUNC(xdr_time_how) 
    SYMBOL_ITEM_FUNC(xdr_uid3) 
    SYMBOL_ITEM_FUNC(xdr_uint32) 
    SYMBOL_ITEM_FUNC(xdr_uint64) 
    SYMBOL_ITEM_FUNC(xdr_wcc_attr) 
    SYMBOL_ITEM_FUNC(xdr_wcc_data) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3args) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3res) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3resok) 
    SYMBOL_ITEM_FUNC(xdr_writeverf3) 
    SYMBOL_ITEM_FUNC(__blkIoFsDrvInstall) 
    SYMBOL_ITEM_FUNC(API_OemBlkIoCreate) 
    SYMBOL_ITEM_FUNC(API_OemBlkIoDelete) 
    SYMBOL_ITEM_FUNC(__oemAutoMountDelete) 
    SYMBOL_ITEM_FUNC(API_OemDiskGetPath) 
    SYMBOL_ITEM_FUNC(API_OemDiskHotplugEventMessage) 
    SYMBOL_ITEM_FUNC(API_OemDiskMount) 
    SYMBOL_ITEM_FUNC(API_OemDiskMount2) 
    SYMBOL_ITEM_FUNC(API_OemDiskMountEx) 
    SYMBOL_ITEM_FUNC(API_OemDiskMountEx2) 
    SYMBOL_ITEM_FUNC(API_OemDiskMountInit) 
    SYMBOL_ITEM_FUNC(API_OemDiskMountShow) 
    SYMBOL_ITEM_FUNC(API_OemDiskRemount) 
    SYMBOL_ITEM_FUNC(API_OemDiskRemountEx) 
    SYMBOL_ITEM_FUNC(API_OemDiskUnmount) 
    SYMBOL_ITEM_FUNC(API_OemDiskUnmountEx) 
    SYMBOL_ITEM_FUNC(API_OemFdisk) 
    SYMBOL_ITEM_FUNC(API_OemFdiskGet) 
    SYMBOL_ITEM_FUNC(API_OemFdiskShow) 
    SYMBOL_ITEM_FUNC(API_ProcFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_ProcFsDrvInstall) 
    SYMBOL_ITEM_FUNC(__procFsFindNode) 
    SYMBOL_ITEM_FUNC(__procFsRemoveNode) 
    SYMBOL_ITEM_FUNC(API_ProcFsAllocNodeBuffer) 
    SYMBOL_ITEM_FUNC(API_ProcFsFreeNodeBuffer) 
    SYMBOL_ITEM_FUNC(API_ProcFsMakeNode) 
    SYMBOL_ITEM_FUNC(API_ProcFsNodeBuffer) 
    SYMBOL_ITEM_FUNC(API_ProcFsNodeBufferSize) 
    SYMBOL_ITEM_FUNC(API_ProcFsNodeGetRealFileSize) 
    SYMBOL_ITEM_FUNC(API_ProcFsNodeMessageValue) 
    SYMBOL_ITEM_FUNC(API_ProcFsNodeSetRealFileSize) 
    SYMBOL_ITEM_FUNC(API_ProcFsRemoveNode) 
    SYMBOL_ITEM_FUNC(__procFsBspInfoInit) 
    SYMBOL_ITEM_FUNC(__procFssupInit) 
    SYMBOL_ITEM_FUNC(__procFsHookInit) 
    SYMBOL_ITEM_FUNC(__procFsKernelInfoInit) 
    SYMBOL_ITEM_FUNC(__procFsPowerInit) 
    SYMBOL_ITEM_FUNC(API_RamFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_RamFsDevDelete) 
    SYMBOL_ITEM_FUNC(API_RamFsDrvInstall) 
    SYMBOL_ITEM_FUNC(__ram_close) 
    SYMBOL_ITEM_FUNC(__ram_increase) 
    SYMBOL_ITEM_FUNC(__ram_maken) 
    SYMBOL_ITEM_FUNC(__ram_mount) 
    SYMBOL_ITEM_FUNC(__ram_move) 
    SYMBOL_ITEM_FUNC(__ram_open) 
    SYMBOL_ITEM_FUNC(__ram_read) 
    SYMBOL_ITEM_FUNC(__ram_stat) 
    SYMBOL_ITEM_FUNC(__ram_statfs) 
    SYMBOL_ITEM_FUNC(__ram_truncate) 
    SYMBOL_ITEM_FUNC(__ram_unlink) 
    SYMBOL_ITEM_FUNC(__ram_unmount) 
    SYMBOL_ITEM_FUNC(__ram_write) 
    SYMBOL_ITEM_FUNC(API_RomFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_RomFsDevDelete) 
    SYMBOL_ITEM_FUNC(API_RomFsDrvInstall) 
    SYMBOL_ITEM_FUNC(__rfs_getfile) 
    SYMBOL_ITEM_FUNC(__rfs_lookup) 
    SYMBOL_ITEM_FUNC(__rfs_mount) 
    SYMBOL_ITEM_FUNC(__rfs_open) 
    SYMBOL_ITEM_FUNC(__rfs_path_build_link) 
    SYMBOL_ITEM_FUNC(__rfs_pread) 
    SYMBOL_ITEM_FUNC(__rfs_readlink) 
    SYMBOL_ITEM_FUNC(__rootFsFixName) 
    SYMBOL_ITEM_FUNC(__rootFsTimeSet) 
    SYMBOL_ITEM_FUNC(API_RootFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_RootFsDrvInstall) 
    SYMBOL_ITEM_FUNC(API_RootFsTime) 
    SYMBOL_ITEM_FUNC(__rootFsDevMatch) 
    SYMBOL_ITEM_FUNC(__rootFsFindNode) 
    SYMBOL_ITEM_FUNC(__rootFsReadNode) 
    SYMBOL_ITEM_FUNC(API_RootFsMakeNode) 
    SYMBOL_ITEM_FUNC(API_RootFsRemoveNode) 
    SYMBOL_ITEM_FUNC(API_RootFsMap) 
    SYMBOL_ITEM_FUNC(API_RootFsMapInit) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeAdjustBP) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeAllocBlk) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeAppendBlk) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeBlkCnt) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeDump) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeFreeBlk) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeGetBlk) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeGetBlkCnt) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeGetLevel) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeGetNode) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeInit) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeInitBP) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeInsertNode) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeReadBP) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeRemoveNode) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeTrunc) 
    SYMBOL_ITEM_FUNC(tpsFsInodeDump) 
    SYMBOL_ITEM_FUNC(tpsFsDevBufRead) 
    SYMBOL_ITEM_FUNC(tpsFsDevBufSync) 
    SYMBOL_ITEM_FUNC(tpsFsDevBufTrim) 
    SYMBOL_ITEM_FUNC(tpsFsDevBufWrite) 
    SYMBOL_ITEM_FUNC(tpsFsCreateEntry) 
    SYMBOL_ITEM_FUNC(tpsFsEntryFree) 
    SYMBOL_ITEM_FUNC(tpsFsEntryRead) 
    SYMBOL_ITEM_FUNC(tpsFsEntryRemove) 
    SYMBOL_ITEM_FUNC(tpsFsFindEntry) 
    SYMBOL_ITEM_FUNC(tpsFsGetDirSize) 
    SYMBOL_ITEM_FUNC(tpsFsCloseInode) 
    SYMBOL_ITEM_FUNC(tpsFsCreateInode) 
    SYMBOL_ITEM_FUNC(tpsFsFlushInodeHead) 
    SYMBOL_ITEM_FUNC(tpsFsInodeAddRef) 
    SYMBOL_ITEM_FUNC(tpsFsInodeAllocBlk) 
    SYMBOL_ITEM_FUNC(tpsFsInodeBuffInvalid) 
    SYMBOL_ITEM_FUNC(tpsFsInodeDelRef) 
    SYMBOL_ITEM_FUNC(tpsFsInodeGetSize) 
    SYMBOL_ITEM_FUNC(tpsFsInodeRead) 
    SYMBOL_ITEM_FUNC(tpsFsInodeSync) 
    SYMBOL_ITEM_FUNC(tpsFsInodeWrite) 
    SYMBOL_ITEM_FUNC(tpsFsOpenInode) 
    SYMBOL_ITEM_FUNC(tpsFsTruncInode) 
    SYMBOL_ITEM_FUNC(tpsFsFlushSuperBlock) 
    SYMBOL_ITEM_FUNC(tpsFsFormat) 
    SYMBOL_ITEM_FUNC(tpsFsMount) 
    SYMBOL_ITEM_FUNC(tpsFsUnmount) 
    SYMBOL_ITEM_FUNC(API_TpsFsDevCreate) 
    SYMBOL_ITEM_FUNC(API_TpsFsDevDelete) 
    SYMBOL_ITEM_FUNC(API_TpsFsDrvInstall) 
    SYMBOL_ITEM_FUNC(__tpsFsLoadTransData) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeTransFini) 
    SYMBOL_ITEM_FUNC(tpsFsBtreeTransInit) 
    SYMBOL_ITEM_FUNC(tpsFsTransAllocAndInit) 
    SYMBOL_ITEM_FUNC(tpsFsTransCommitAndFree) 
    SYMBOL_ITEM_FUNC(tpsFsTransRead) 
    SYMBOL_ITEM_FUNC(tpsFsTransRollBackAndFree) 
    SYMBOL_ITEM_FUNC(tpsFsTransTrigerChk) 
    SYMBOL_ITEM_FUNC(tpsFsTransWrite) 
    SYMBOL_ITEM_FUNC(tspFsCheckTrans) 
    SYMBOL_ITEM_FUNC(tspFsCompleteTrans) 
    SYMBOL_ITEM_FUNC(tpsFsChmod) 
    SYMBOL_ITEM_FUNC(tpsFsChown) 
    SYMBOL_ITEM_FUNC(tpsFsChtime) 
    SYMBOL_ITEM_FUNC(tpsFsClose) 
    SYMBOL_ITEM_FUNC(tpsFsFlushHead) 
    SYMBOL_ITEM_FUNC(tpsFsFlushInodes) 
    SYMBOL_ITEM_FUNC(tpsFsGetmod) 
    SYMBOL_ITEM_FUNC(tpsFsGetSize) 
    SYMBOL_ITEM_FUNC(tpsFsLink) 
    SYMBOL_ITEM_FUNC(tpsFsMkDir) 
    SYMBOL_ITEM_FUNC(tpsFsMove) 
    SYMBOL_ITEM_FUNC(tpsFsOpen) 
    SYMBOL_ITEM_FUNC(tpsFsRead) 
    SYMBOL_ITEM_FUNC(tpsFsReadDir) 
    SYMBOL_ITEM_FUNC(tpsFsRemove) 
    SYMBOL_ITEM_FUNC(tpsFsStat) 
    SYMBOL_ITEM_FUNC(tpsFsStatfs) 
    SYMBOL_ITEM_FUNC(tpsFsSync) 
    SYMBOL_ITEM_FUNC(tpsFsTrunc) 
    SYMBOL_ITEM_FUNC(tpsFsVolSync) 
    SYMBOL_ITEM_FUNC(tpsFsWrite) 
    SYMBOL_ITEM_FUNC(API_TSyncAdd) 
    SYMBOL_ITEM_FUNC(API_FsUniqueAlloc) 
    SYMBOL_ITEM_FUNC(API_FsUniqueCreate) 
    SYMBOL_ITEM_FUNC(API_FsUniqueDelete) 
    SYMBOL_ITEM_FUNC(API_FsUniqueFree) 
    SYMBOL_ITEM_FUNC(API_FsUniqueIsVal) 
    SYMBOL_ITEM_FUNC(yaffs_alloc_raw_obj) 
    SYMBOL_ITEM_FUNC(yaffs_alloc_raw_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_deinit_raw_tnodes_and_objs) 
    SYMBOL_ITEM_FUNC(yaffs_free_raw_obj) 
    SYMBOL_ITEM_FUNC(yaffs_free_raw_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_init_raw_tnodes_and_objs) 
    SYMBOL_ITEM_FUNC(yaffs_attribs_init) 
    SYMBOL_ITEM_FUNC(yaffs_get_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_load_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_load_attribs_oh) 
    SYMBOL_ITEM_FUNC(yaffs_load_current_time) 
    SYMBOL_ITEM_FUNC(yaffs_set_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_check_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_clear_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_clear_chunk_bits) 
    SYMBOL_ITEM_FUNC(yaffs_count_chunk_bits) 
    SYMBOL_ITEM_FUNC(yaffs_set_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_still_some_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_verify_chunk_bit_id) 
    SYMBOL_ITEM_FUNC(yaffs_checkpt_close) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_invalidate_stream) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_open) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_rd) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_wr) 
    SYMBOL_ITEM_FUNC(yaffs2_get_checkpt_sum) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_calc) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_calc_other) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_correct) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_correct_other) 
    SYMBOL_ITEM_FUNC(yaffs_add_find_tnode_0) 
    SYMBOL_ITEM_FUNC(yaffs_add_obj_to_dir) 
    SYMBOL_ITEM_FUNC(yaffs_addr_to_chunk) 
    SYMBOL_ITEM_FUNC(yaffs_bg_gc) 
    SYMBOL_ITEM_FUNC(yaffs_block_became_dirty) 
    SYMBOL_ITEM_FUNC(yaffs_check_alloc_available) 
    SYMBOL_ITEM_FUNC(yaffs_check_ff) 
    SYMBOL_ITEM_FUNC(yaffs_chunk_del) 
    SYMBOL_ITEM_FUNC(yaffs_clone_str) 
    SYMBOL_ITEM_FUNC(yaffs_count_blocks_by_state) 
    SYMBOL_ITEM_FUNC(yaffs_count_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_create_dir) 
    SYMBOL_ITEM_FUNC(yaffs_create_file) 
    SYMBOL_ITEM_FUNC(yaffs_create_special) 
    SYMBOL_ITEM_FUNC(yaffs_create_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_deinitialise) 
    SYMBOL_ITEM_FUNC(yaffs_del_obj) 
    SYMBOL_ITEM_FUNC(yaffs_do_file_wr) 
    SYMBOL_ITEM_FUNC(yaffs_file_rd) 
    SYMBOL_ITEM_FUNC(yaffs_find_by_name) 
    SYMBOL_ITEM_FUNC(yaffs_find_by_number) 
    SYMBOL_ITEM_FUNC(yaffs_find_chunk_in_file) 
    SYMBOL_ITEM_FUNC(yaffs_find_or_create_by_number) 
    SYMBOL_ITEM_FUNC(yaffs_find_tnode_0) 
    SYMBOL_ITEM_FUNC(yaffs_flush_file) 
    SYMBOL_ITEM_FUNC(yaffs_flush_whole_cache) 
    SYMBOL_ITEM_FUNC(yaffs_get_equivalent_obj) 
    SYMBOL_ITEM_FUNC(yaffs_get_group_base) 
    SYMBOL_ITEM_FUNC(yaffs_get_n_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_inode) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_length) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_link_count) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_name) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_type) 
    SYMBOL_ITEM_FUNC(yaffs_get_symlink_alias) 
    SYMBOL_ITEM_FUNC(yaffs_get_temp_buffer) 
    SYMBOL_ITEM_FUNC(yaffs_get_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_get_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_guts_format_dev) 
    SYMBOL_ITEM_FUNC(yaffs_guts_initialise) 
    SYMBOL_ITEM_FUNC(yaffs_guts_ll_init) 
    SYMBOL_ITEM_FUNC(yaffs_handle_chunk_error) 
    SYMBOL_ITEM_FUNC(yaffs_handle_defered_free) 
    SYMBOL_ITEM_FUNC(yaffs_handle_shadowed_obj) 
    SYMBOL_ITEM_FUNC(yaffs_is_non_empty_dir) 
    SYMBOL_ITEM_FUNC(yaffs_link_fixup) 
    SYMBOL_ITEM_FUNC(yaffs_link_obj) 
    SYMBOL_ITEM_FUNC(yaffs_list_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_lost_n_found) 
    SYMBOL_ITEM_FUNC(yaffs_max_file_size) 
    SYMBOL_ITEM_FUNC(yaffs_oh_size_load) 
    SYMBOL_ITEM_FUNC(yaffs_oh_to_size) 
    SYMBOL_ITEM_FUNC(yaffs_put_chunk_in_file) 
    SYMBOL_ITEM_FUNC(yaffs_release_temp_buffer) 
    SYMBOL_ITEM_FUNC(yaffs_remove_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_rename_obj) 
    SYMBOL_ITEM_FUNC(yaffs_resize_file) 
    SYMBOL_ITEM_FUNC(yaffs_resize_file_down) 
    SYMBOL_ITEM_FUNC(yaffs_retype_obj) 
    SYMBOL_ITEM_FUNC(yaffs_root) 
    SYMBOL_ITEM_FUNC(yaffs_set_obj_name) 
    SYMBOL_ITEM_FUNC(yaffs_set_obj_name_from_oh) 
    SYMBOL_ITEM_FUNC(yaffs_set_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_skip_rest_of_block) 
    SYMBOL_ITEM_FUNC(yaffs_unlink_obj) 
    SYMBOL_ITEM_FUNC(yaffs_unlinker) 
    SYMBOL_ITEM_FUNC(yaffs_update_dirty_dirs) 
    SYMBOL_ITEM_FUNC(yaffs_update_oh) 
    SYMBOL_ITEM_FUNC(yaffs_wr_file) 
    SYMBOL_ITEM_FUNC(nandmtd_erase_block) 
    SYMBOL_ITEM_FUNC(yaffs_get_mtd_device) 
    SYMBOL_ITEM_FUNC(yaffs_mtd_drv_install) 
    SYMBOL_ITEM_FUNC(yaffs_put_mtd_device) 
    SYMBOL_ITEM_FUNC(yaffs_verify_mtd) 
    SYMBOL_ITEM_FUNC(nval_del) 
    SYMBOL_ITEM_FUNC(nval_get) 
    SYMBOL_ITEM_FUNC(nval_hasvalues) 
    SYMBOL_ITEM_FUNC(nval_list) 
    SYMBOL_ITEM_FUNC(nval_set) 
    SYMBOL_ITEM_FUNC(yaffs_deinit_nand) 
    SYMBOL_ITEM_FUNC(yaffs_erase_block) 
    SYMBOL_ITEM_FUNC(yaffs_init_nand) 
    SYMBOL_ITEM_FUNC(yaffs_mark_bad) 
    SYMBOL_ITEM_FUNC(yaffs_query_init_block_state) 
    SYMBOL_ITEM_FUNC(yaffs_rd_chunk_tags_nand) 
    SYMBOL_ITEM_FUNC(yaffs_wr_chunk_tags_nand) 
    SYMBOL_ITEM_FUNC(ynorDeinitialise) 
    SYMBOL_ITEM_FUNC(ynorEraseBlock) 
    SYMBOL_ITEM_FUNC(ynorInitialise) 
    SYMBOL_ITEM_FUNC(ynorReadChunk) 
    SYMBOL_ITEM_FUNC(ynorWriteChunk) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags1) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags1) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags2) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags2_tags_only) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags2) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags2_tags_only) 
    SYMBOL_ITEM_FUNC(yaffs_summary_add) 
    SYMBOL_ITEM_FUNC(yaffs_summary_deinit) 
    SYMBOL_ITEM_FUNC(yaffs_summary_fetch) 
    SYMBOL_ITEM_FUNC(yaffs_summary_gc) 
    SYMBOL_ITEM_FUNC(yaffs_summary_init) 
    SYMBOL_ITEM_FUNC(yaffs_summary_read) 
    SYMBOL_ITEM_FUNC(__yaffsOsInit) 
    SYMBOL_ITEM_FUNC(API_YaffsDevCreate) 
    SYMBOL_ITEM_FUNC(API_YaffsDevDelete) 
    SYMBOL_ITEM_FUNC(API_YaffsDevMountShow) 
    SYMBOL_ITEM_FUNC(API_YaffsDevSync) 
    SYMBOL_ITEM_FUNC(API_YaffsDrvInstall) 
    SYMBOL_ITEM_FUNC(__procFsYaffsInit) 
    SYMBOL_ITEM_FUNC(yaffs_calc_tags_ecc) 
    SYMBOL_ITEM_FUNC(yaffs_check_tags_ecc) 
    SYMBOL_ITEM_FUNC(yaffs_tags_compat_install) 
    SYMBOL_ITEM_FUNC(yaffs_tags_marshall_install) 
    SYMBOL_ITEM_FUNC(yaffs_skip_verification) 
    SYMBOL_ITEM_FUNC(yaffs_verify_blk) 
    SYMBOL_ITEM_FUNC(yaffs_verify_blocks) 
    SYMBOL_ITEM_FUNC(yaffs_verify_collected_blk) 
    SYMBOL_ITEM_FUNC(yaffs_verify_dir) 
    SYMBOL_ITEM_FUNC(yaffs_verify_file) 
    SYMBOL_ITEM_FUNC(yaffs_verify_file_sane) 
    SYMBOL_ITEM_FUNC(yaffs_verify_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_verify_link) 
    SYMBOL_ITEM_FUNC(yaffs_verify_obj) 
    SYMBOL_ITEM_FUNC(yaffs_verify_obj_in_dir) 
    SYMBOL_ITEM_FUNC(yaffs_verify_objects) 
    SYMBOL_ITEM_FUNC(yaffs_verify_oh) 
    SYMBOL_ITEM_FUNC(yaffs_verify_special) 
    SYMBOL_ITEM_FUNC(yaffs_verify_symlink) 
    SYMBOL_ITEM_FUNC(yaffs1_scan) 
    SYMBOL_ITEM_FUNC(yaffs_block_ok_for_gc) 
    SYMBOL_ITEM_FUNC(yaffs_calc_checkpt_blocks_required) 
    SYMBOL_ITEM_FUNC(yaffs_calc_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs_checkpoint_save) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_invalidate) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_required) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_restore) 
    SYMBOL_ITEM_FUNC(yaffs2_clear_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs2_find_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs2_find_refresh_block) 
    SYMBOL_ITEM_FUNC(yaffs2_handle_hole) 
    SYMBOL_ITEM_FUNC(yaffs2_scan_backwards) 
    SYMBOL_ITEM_FUNC(yaffs2_update_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs_getdevname) 
    SYMBOL_ITEM_FUNC(yaffs_start_up) 
    SYMBOL_ITEM_FUNC(yaffsfs_CheckMemRegion) 
    SYMBOL_ITEM_FUNC(yaffsfs_CurrentTime) 
    SYMBOL_ITEM_FUNC(yaffsfs_FindSymlink) 
    SYMBOL_ITEM_FUNC(yaffsfs_free) 
    SYMBOL_ITEM_FUNC(yaffsfs_GetLastError) 
    SYMBOL_ITEM_FUNC(yaffsfs_Lock) 
    SYMBOL_ITEM_FUNC(yaffsfs_malloc) 
    SYMBOL_ITEM_FUNC(yaffsfs_OSInitialisation) 
    SYMBOL_ITEM_FUNC(yaffsfs_PathBuildLink) 
    SYMBOL_ITEM_FUNC(yaffsfs_SetError) 
    SYMBOL_ITEM_FUNC(yaffsfs_Unlock) 
    SYMBOL_ITEM_FUNC(yaffs_access) 
    SYMBOL_ITEM_FUNC(yaffs_access_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_access_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_add_device) 
    SYMBOL_ITEM_FUNC(yaffs_chmod) 
    SYMBOL_ITEM_FUNC(yaffs_chmod_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_chmod_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_close) 
    SYMBOL_ITEM_FUNC(yaffs_closedir) 
    SYMBOL_ITEM_FUNC(yaffs_dev_rewind) 
    SYMBOL_ITEM_FUNC(yaffs_do_background_gc) 
    SYMBOL_ITEM_FUNC(yaffs_do_background_gc_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_dump_dev) 
    SYMBOL_ITEM_FUNC(yaffs_dump_dev_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_dup) 
    SYMBOL_ITEM_FUNC(yaffs_fchmod) 
    SYMBOL_ITEM_FUNC(yaffs_fdatasync) 
    SYMBOL_ITEM_FUNC(yaffs_fgetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_flistxattr) 
    SYMBOL_ITEM_FUNC(yaffs_flush) 
    SYMBOL_ITEM_FUNC(yaffs_format) 
    SYMBOL_ITEM_FUNC(yaffs_format_common) 
    SYMBOL_ITEM_FUNC(yaffs_format_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_freespace) 
    SYMBOL_ITEM_FUNC(yaffs_freespace_common) 
    SYMBOL_ITEM_FUNC(yaffs_freespace_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_fremovexattr) 
    SYMBOL_ITEM_FUNC(yaffs_fsetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_fstat) 
    SYMBOL_ITEM_FUNC(yaffs_fsync) 
    SYMBOL_ITEM_FUNC(yaffs_ftruncate) 
    SYMBOL_ITEM_FUNC(yaffs_futime) 
    SYMBOL_ITEM_FUNC(yaffs_get_error) 
    SYMBOL_ITEM_FUNC(yaffs_get_trace) 
    SYMBOL_ITEM_FUNC(yaffs_getdev) 
    SYMBOL_ITEM_FUNC(yaffs_getxattr) 
    SYMBOL_ITEM_FUNC(yaffs_getxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_getxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_handle_rw_set) 
    SYMBOL_ITEM_FUNC(yaffs_inodecount) 
    SYMBOL_ITEM_FUNC(yaffs_inodecount_common) 
    SYMBOL_ITEM_FUNC(yaffs_inodecount_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_lgetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_lgetxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_lgetxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_link) 
    SYMBOL_ITEM_FUNC(yaffs_link_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_listxattr) 
    SYMBOL_ITEM_FUNC(yaffs_listxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_listxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_llistxattr) 
    SYMBOL_ITEM_FUNC(yaffs_llistxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_llistxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_lremovexattr) 
    SYMBOL_ITEM_FUNC(yaffs_lremovexattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_lremovexattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_lseek) 
    SYMBOL_ITEM_FUNC(yaffs_lsetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_lsetxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_lsetxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_lstat) 
    SYMBOL_ITEM_FUNC(yaffs_lstat_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_lstat_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_mkdir) 
    SYMBOL_ITEM_FUNC(yaffs_mkdir_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_mkdir_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_mknod) 
    SYMBOL_ITEM_FUNC(yaffs_mknod_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_mknod_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_mount) 
    SYMBOL_ITEM_FUNC(yaffs_mount_common) 
    SYMBOL_ITEM_FUNC(yaffs_mount_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_mount2) 
    SYMBOL_ITEM_FUNC(yaffs_mount2_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_mount3) 
    SYMBOL_ITEM_FUNC(yaffs_mount3_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_n_handles) 
    SYMBOL_ITEM_FUNC(yaffs_n_handles_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_next_dev) 
    SYMBOL_ITEM_FUNC(yaffs_open) 
    SYMBOL_ITEM_FUNC(yaffs_open_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_open_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_open_sharing) 
    SYMBOL_ITEM_FUNC(yaffs_open_sharing_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_open_sharing_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_opendir) 
    SYMBOL_ITEM_FUNC(yaffs_opendir_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_opendir_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_pread) 
    SYMBOL_ITEM_FUNC(yaffs_pwrite) 
    SYMBOL_ITEM_FUNC(yaffs_read) 
    SYMBOL_ITEM_FUNC(yaffs_readdir) 
    SYMBOL_ITEM_FUNC(yaffs_readdir_fd) 
    SYMBOL_ITEM_FUNC(yaffs_readlink) 
    SYMBOL_ITEM_FUNC(yaffs_readlink_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_remount) 
    SYMBOL_ITEM_FUNC(yaffs_remount_common) 
    SYMBOL_ITEM_FUNC(yaffs_remount_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_remove_device) 
    SYMBOL_ITEM_FUNC(yaffs_removexattr) 
    SYMBOL_ITEM_FUNC(yaffs_removexattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_removexattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_rename) 
    SYMBOL_ITEM_FUNC(yaffs_rename_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_rename_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_rewinddir) 
    SYMBOL_ITEM_FUNC(yaffs_rewinddir_fd) 
    SYMBOL_ITEM_FUNC(yaffs_rmdir) 
    SYMBOL_ITEM_FUNC(yaffs_rmdir_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_rmdir_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_set_error) 
    SYMBOL_ITEM_FUNC(yaffs_set_trace) 
    SYMBOL_ITEM_FUNC(yaffs_setxattr) 
    SYMBOL_ITEM_FUNC(yaffs_setxattr_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_setxattr_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_stat) 
    SYMBOL_ITEM_FUNC(yaffs_stat_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_stat_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_symlink_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_sync) 
    SYMBOL_ITEM_FUNC(yaffs_sync_common) 
    SYMBOL_ITEM_FUNC(yaffs_sync_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_totalspace) 
    SYMBOL_ITEM_FUNC(yaffs_totalspace_common) 
    SYMBOL_ITEM_FUNC(yaffs_totalspace_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_truncate) 
    SYMBOL_ITEM_FUNC(yaffs_truncate_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_truncate_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_unlink) 
    SYMBOL_ITEM_FUNC(yaffs_unlink_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_unlink_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_unmount) 
    SYMBOL_ITEM_FUNC(yaffs_unmount_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_unmount2) 
    SYMBOL_ITEM_FUNC(yaffs_unmount2_common) 
    SYMBOL_ITEM_FUNC(yaffs_unmount2_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_utime) 
    SYMBOL_ITEM_FUNC(yaffs_utime_reldev) 
    SYMBOL_ITEM_FUNC(yaffs_utime_reldir) 
    SYMBOL_ITEM_FUNC(yaffs_write) 
    SYMBOL_ITEM_FUNC(yaffsfs_readdir_no_lock) 
    SYMBOL_ITEM_FUNC(yaffs_hweight32) 
    SYMBOL_ITEM_FUNC(yaffs_hweight8) 
    SYMBOL_ITEM_FUNC(yaffs_qsort) 
    SYMBOL_ITEM_FUNC(API_GuiInputDevKeyboardHookSet) 
    SYMBOL_ITEM_FUNC(API_GuiInputDevMouseHookSet) 
    SYMBOL_ITEM_FUNC(API_GuiInputDevProcStart) 
    SYMBOL_ITEM_FUNC(API_GuiInputDevProcStop) 
    SYMBOL_ITEM_FUNC(API_GuiInputDevReg) 
    SYMBOL_ITEM_FUNC(API_CacheAliasProb) 
    SYMBOL_ITEM_FUNC(API_CacheBarrier) 
    SYMBOL_ITEM_FUNC(API_CacheClear) 
    SYMBOL_ITEM_FUNC(API_CacheClearPage) 
    SYMBOL_ITEM_FUNC(API_CacheDataUpdate) 
    SYMBOL_ITEM_FUNC(API_CacheDisable) 
    SYMBOL_ITEM_FUNC(API_CacheDmaClear) 
    SYMBOL_ITEM_FUNC(API_CacheDmaFlush) 
    SYMBOL_ITEM_FUNC(API_CacheDmaFree) 
    SYMBOL_ITEM_FUNC(API_CacheDmaInvalidate) 
    SYMBOL_ITEM_FUNC(API_CacheDmaMalloc) 
    SYMBOL_ITEM_FUNC(API_CacheDmaMallocAlign) 
    SYMBOL_ITEM_FUNC(API_CacheEnable) 
    SYMBOL_ITEM_FUNC(API_CacheFlush) 
    SYMBOL_ITEM_FUNC(API_CacheFlushPage) 
    SYMBOL_ITEM_FUNC(API_CacheFuncsSet) 
    SYMBOL_ITEM_FUNC(API_CacheGetLibBlock) 
    SYMBOL_ITEM_FUNC(API_CacheGetMode) 
    SYMBOL_ITEM_FUNC(API_CacheGetOption) 
    SYMBOL_ITEM_FUNC(API_CacheInvalidate) 
    SYMBOL_ITEM_FUNC(API_CacheInvalidatePage) 
    SYMBOL_ITEM_FUNC(API_CacheLibPrimaryInit) 
    SYMBOL_ITEM_FUNC(API_CacheLibSecondaryInit) 
    SYMBOL_ITEM_FUNC(API_CacheLine) 
    SYMBOL_ITEM_FUNC(API_CacheLocalTextUpdate) 
    SYMBOL_ITEM_FUNC(API_CacheLocation) 
    SYMBOL_ITEM_FUNC(API_CacheLock) 
    SYMBOL_ITEM_FUNC(API_CacheTextUpdate) 
    SYMBOL_ITEM_FUNC(API_CacheUnlock) 
    SYMBOL_ITEM_FUNC(API_CacheWaySize) 
    SYMBOL_ITEM_FUNC(API_CrashDumpBuffer) 
    SYMBOL_ITEM_FUNC(API_CrashDumpSave) 
    SYMBOL_ITEM_FUNC(API_CrashDumpShow) 
    SYMBOL_ITEM_FUNC(_CrashDumpAbortAccess) 
    SYMBOL_ITEM_FUNC(_CrashDumpAbortFatal) 
    SYMBOL_ITEM_FUNC(_CrashDumpAbortKernel) 
    SYMBOL_ITEM_FUNC(_CrashDumpAbortStkOf) 
    SYMBOL_ITEM_FUNC(_CrashDumpGet) 
    SYMBOL_ITEM_FUNC(_CrashDumpSet) 
    SYMBOL_ITEM_FUNC(_BitmapAdd) 
    SYMBOL_ITEM_FUNC(_BitmapDel) 
    SYMBOL_ITEM_FUNC(_BitmapHigh) 
    SYMBOL_ITEM_FUNC(_BitmapInit) 
    SYMBOL_ITEM_FUNC(_BitmapIsEmpty) 
    SYMBOL_ITEM_FUNC(_CandTableTryAdd) 
    SYMBOL_ITEM_FUNC(_CandTableTryDel) 
    SYMBOL_ITEM_FUNC(_CandTableUpdate) 
    SYMBOL_ITEM_FUNC(_CoroutineFreeAll) 
    SYMBOL_ITEM_FUNC(_CoroutineReclaim) 
    SYMBOL_ITEM_FUNC(_CoroutineShell) 
    SYMBOL_ITEM_FUNC(_CpuActive) 
    SYMBOL_ITEM_FUNC(_CpuGetMaxNesting) 
    SYMBOL_ITEM_FUNC(_CpuGetNesting) 
    SYMBOL_ITEM_FUNC(_CpuGetSchedAffinity) 
    SYMBOL_ITEM_FUNC(_CpuSetSchedAffinity) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(_DebugFmtMsg) 
    SYMBOL_ITEM_FUNC(_DebugMessage) 
    SYMBOL_ITEM_FUNC(_ErrorHandle) 
    SYMBOL_ITEM_FUNC(_EventPrioTryBoost) 
    SYMBOL_ITEM_FUNC(_EventPrioTryResume) 
    SYMBOL_ITEM_FUNC(_EventReadyFifoLowLevel) 
    SYMBOL_ITEM_FUNC(_EventReadyHighLevel) 
    SYMBOL_ITEM_FUNC(_EventReadyPriorityLowLevel) 
    SYMBOL_ITEM_FUNC(_EventUnQueue) 
    SYMBOL_ITEM_FUNC(_EventWaitFifo) 
    SYMBOL_ITEM_FUNC(_EventWaitPriority) 
    SYMBOL_ITEM_FUNC(_EventInit) 
    SYMBOL_ITEM_FUNC(_AddTCBToEventFifo) 
    SYMBOL_ITEM_FUNC(_AddTCBToEventPriority) 
    SYMBOL_ITEM_FUNC(_DelTCBFromEventFifo) 
    SYMBOL_ITEM_FUNC(_DelTCBFromEventPriority) 
    SYMBOL_ITEM_FUNC(_EventSetBlock) 
    SYMBOL_ITEM_FUNC(_EventSetInit) 
    SYMBOL_ITEM_FUNC(_EventSetDeleteReady) 
    SYMBOL_ITEM_FUNC(_EventSetThreadReady) 
    SYMBOL_ITEM_FUNC(_EventSetUnQueue) 
    SYMBOL_ITEM_FUNC(_GlobalPrimaryInit) 
    SYMBOL_ITEM_FUNC(_GlobalSecondaryInit) 
    SYMBOL_ITEM_FUNC(_HeapInit) 
    SYMBOL_ITEM_FUNC(_HeapKernelInit) 
    SYMBOL_ITEM_FUNC(_HeapSystemInit) 
    SYMBOL_ITEM_FUNC(_HeapAddMemory) 
    SYMBOL_ITEM_FUNC(_HeapAllocate) 
    SYMBOL_ITEM_FUNC(_HeapAllocateAlign) 
    SYMBOL_ITEM_FUNC(_HeapCreate) 
    SYMBOL_ITEM_FUNC(_HeapCtor) 
    SYMBOL_ITEM_FUNC(_HeapCtorEx) 
    SYMBOL_ITEM_FUNC(_HeapDelete) 
    SYMBOL_ITEM_FUNC(_HeapDtor) 
    SYMBOL_ITEM_FUNC(_HeapFree) 
    SYMBOL_ITEM_FUNC(_HeapGetInfo) 
    SYMBOL_ITEM_FUNC(_HeapGetMax) 
    SYMBOL_ITEM_FUNC(_HeapRealloc) 
    SYMBOL_ITEM_FUNC(_HeapTraceAlloc) 
    SYMBOL_ITEM_FUNC(_HeapTraceFree) 
    SYMBOL_ITEM_FUNC(_HeapVerify) 
    SYMBOL_ITEM_FUNC(_HeapZallocate) 
    SYMBOL_ITEM_FUNC(_IdleThread) 
    SYMBOL_ITEM_FUNC(_InterVectInit) 
    SYMBOL_ITEM_FUNC(_ITimerThread) 
    SYMBOL_ITEM_FUNC(_ITimerWakeup) 
    SYMBOL_ITEM_FUNC(_jobQueueAdd) 
    SYMBOL_ITEM_FUNC(_jobQueueCreate) 
    SYMBOL_ITEM_FUNC(_jobQueueDel) 
    SYMBOL_ITEM_FUNC(_jobQueueDelete) 
    SYMBOL_ITEM_FUNC(_jobQueueExec) 
    SYMBOL_ITEM_FUNC(_jobQueueFinit) 
    SYMBOL_ITEM_FUNC(_jobQueueFlush) 
    SYMBOL_ITEM_FUNC(_jobQueueInit) 
    SYMBOL_ITEM_FUNC(_jobQueueLost) 
    SYMBOL_ITEM_FUNC(_KernelHighLevelInit) 
    SYMBOL_ITEM_FUNC(_KernelPrimaryLowLevelInit) 
    SYMBOL_ITEM_FUNC(_KernelSecondaryLowLevelInit) 
    SYMBOL_ITEM_FUNC(__kernelEnter) 
    SYMBOL_ITEM_FUNC(__kernelEnterFunc) 
    SYMBOL_ITEM_FUNC(__kernelEnterIrq) 
    SYMBOL_ITEM_FUNC(__kernelExit) 
    SYMBOL_ITEM_FUNC(__kernelExitIrq) 
    SYMBOL_ITEM_FUNC(__kernelIsEnter) 
    SYMBOL_ITEM_FUNC(__kernelIsLockByMe) 
    SYMBOL_ITEM_FUNC(__kernelOwner) 
    SYMBOL_ITEM_FUNC(__kernelSched) 
    SYMBOL_ITEM_FUNC(__kernelSchedInt) 
    SYMBOL_ITEM_FUNC(__kernelSpaceEnter) 
    SYMBOL_ITEM_FUNC(__kernelSpaceExit) 
    SYMBOL_ITEM_FUNC(__kernelSpaceGet) 
    SYMBOL_ITEM_FUNC(__kernelSpaceGet2) 
    SYMBOL_ITEM_FUNC(__kernelSpaceSet) 
    SYMBOL_ITEM_FUNC(__kernelSpaceSet2) 
    SYMBOL_ITEM_FUNC(_MsgQueueInit) 
    SYMBOL_ITEM_FUNC(_MsgQueueClear) 
    SYMBOL_ITEM_FUNC(_MsgQueueGet) 
    SYMBOL_ITEM_FUNC(_MsgQueueMsgLen) 
    SYMBOL_ITEM_FUNC(_MsgQueuePut) 
    SYMBOL_ITEM_FUNC(_MakeObjectId) 
    SYMBOL_ITEM_FUNC(_PartitionInit) 
    SYMBOL_ITEM_FUNC(_PartitionAllocate) 
    SYMBOL_ITEM_FUNC(_PartitionFree) 
    SYMBOL_ITEM_FUNC(_PriorityInit) 
    SYMBOL_ITEM_FUNC(_AddTCBToReadyRing) 
    SYMBOL_ITEM_FUNC(_DelTCBFromReadyRing) 
    SYMBOL_ITEM_FUNC(_ReadyTableInit) 
    SYMBOL_ITEM_FUNC(_ReadyTableAdd) 
    SYMBOL_ITEM_FUNC(_ReadyTableDel) 
    SYMBOL_ITEM_FUNC(_RmsInit) 
    SYMBOL_ITEM_FUNC(_RmsActive) 
    SYMBOL_ITEM_FUNC(_RmsEndExpire) 
    SYMBOL_ITEM_FUNC(_RmsGetExecTime) 
    SYMBOL_ITEM_FUNC(_RmsInitExpire) 
    SYMBOL_ITEM_FUNC(_RtcInit) 
    SYMBOL_ITEM_FUNC(_SchedCrSwp) 
    SYMBOL_ITEM_FUNC(_SchedSafeStack) 
    SYMBOL_ITEM_FUNC(_SchedSetPrio) 
    SYMBOL_ITEM_FUNC(_SchedSetRet) 
    SYMBOL_ITEM_FUNC(_SchedSwp) 
    SYMBOL_ITEM_FUNC(_Schedule) 
    SYMBOL_ITEM_FUNC(_ScheduleInit) 
    SYMBOL_ITEM_FUNC(_ScheduleInt) 
    SYMBOL_ITEM_FUNC(_SchedGetCand) 
    SYMBOL_ITEM_FUNC(_SchedIsLock) 
    SYMBOL_ITEM_FUNC(_SchedTick) 
    SYMBOL_ITEM_FUNC(_SchedYield) 
    SYMBOL_ITEM_FUNC(_SmpCallFunc) 
    SYMBOL_ITEM_FUNC(_SmpCallFuncAllOther) 
    SYMBOL_ITEM_FUNC(_SmpPerfIpi) 
    SYMBOL_ITEM_FUNC(_SmpProcIpi) 
    SYMBOL_ITEM_FUNC(_SmpSendIpi) 
    SYMBOL_ITEM_FUNC(_SmpSendIpiAllOther) 
    SYMBOL_ITEM_FUNC(_SmpTryProcIpi) 
    SYMBOL_ITEM_FUNC(_SmpUpdateIpi) 
    SYMBOL_ITEM_FUNC(_SmpSpinInit) 
    SYMBOL_ITEM_FUNC(_SmpSpinLock) 
    SYMBOL_ITEM_FUNC(_SmpSpinLockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinLockIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinLockIrqQuick) 
    SYMBOL_ITEM_FUNC(_SmpSpinLockRaw) 
    SYMBOL_ITEM_FUNC(_SmpSpinLockTask) 
    SYMBOL_ITEM_FUNC(_SmpSpinTryLock) 
    SYMBOL_ITEM_FUNC(_SmpSpinTryLockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinTryLockIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinTryLockRaw) 
    SYMBOL_ITEM_FUNC(_SmpSpinTryLockTask) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlock) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlockIrq) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlockIrqQuick) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlockRaw) 
    SYMBOL_ITEM_FUNC(_SmpSpinUnlockTask) 
    SYMBOL_ITEM_FUNC(_SmpKernelLockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpKernelLockQuick) 
    SYMBOL_ITEM_FUNC(_SmpKernelUnlockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpKernelUnlockQuick) 
    SYMBOL_ITEM_FUNC(_SmpKernelUnlockSched) 
    SYMBOL_ITEM_FUNC(_SmpKernTimeLockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpKernTimeLockQuick) 
    SYMBOL_ITEM_FUNC(_SmpKernTimeUnlockIgnIrq) 
    SYMBOL_ITEM_FUNC(_SmpKernTimeUnlockQuick) 
    SYMBOL_ITEM_FUNC(_StackCheckGuard) 
    SYMBOL_ITEM_FUNC(_StackCheckInit) 
    SYMBOL_ITEM_FUNC(_StackAllocate) 
    SYMBOL_ITEM_FUNC(_StackFree) 
    SYMBOL_ITEM_FUNC(_ThreadGetAffinity) 
    SYMBOL_ITEM_FUNC(_ThreadOffAffinity) 
    SYMBOL_ITEM_FUNC(_ThreadSetAffinity) 
    SYMBOL_ITEM_FUNC(_ThreadFpuSwitch) 
    SYMBOL_ITEM_FUNC(_ThreadIdInit) 
    SYMBOL_ITEM_FUNC(_TCBBuild) 
    SYMBOL_ITEM_FUNC(_TCBBuildExt) 
    SYMBOL_ITEM_FUNC(_TCBCleanupPopExt) 
    SYMBOL_ITEM_FUNC(_TCBDestroy) 
    SYMBOL_ITEM_FUNC(_TCBDestroyExt) 
    SYMBOL_ITEM_FUNC(_TCBTryRun) 
    SYMBOL_ITEM_FUNC(_ThreadDetach) 
    SYMBOL_ITEM_FUNC(_ThreadDisjoin) 
    SYMBOL_ITEM_FUNC(_ThreadJoin) 
    SYMBOL_ITEM_FUNC(_ThreadWjAdd) 
    SYMBOL_ITEM_FUNC(_ThreadWjClear) 
    SYMBOL_ITEM_FUNC(_ThreadContinue) 
    SYMBOL_ITEM_FUNC(_ThreadDebugUnpendSem) 
    SYMBOL_ITEM_FUNC(_ThreadDeleteProcHook) 
    SYMBOL_ITEM_FUNC(_ThreadDeleteWaitDeath) 
    SYMBOL_ITEM_FUNC(_ThreadLock) 
    SYMBOL_ITEM_FUNC(_ThreadMigrateToProc) 
    SYMBOL_ITEM_FUNC(_ThreadRestartProcHook) 
    SYMBOL_ITEM_FUNC(_ThreadSched) 
    SYMBOL_ITEM_FUNC(_ThreadStop) 
    SYMBOL_ITEM_FUNC(_ThreadTraversal) 
    SYMBOL_ITEM_FUNC(_ThreadUnlock) 
    SYMBOL_ITEM_FUNC(_ThreadUserGet) 
    SYMBOL_ITEM_FUNC(_ThreadSafeInKern) 
    SYMBOL_ITEM_FUNC(_ThreadSafeInternal) 
    SYMBOL_ITEM_FUNC(_ThreadSafeResume) 
    SYMBOL_ITEM_FUNC(_ThreadSafeSuspend) 
    SYMBOL_ITEM_FUNC(_ThreadUnsafeInternal) 
    SYMBOL_ITEM_FUNC(_ThreadUnsafeInternalEx) 
    SYMBOL_ITEM_FUNC(_ThreadShell) 
    SYMBOL_ITEM_FUNC(_ThreadStatusChange) 
    SYMBOL_ITEM_FUNC(_ThreadStatusChangeCur) 
    SYMBOL_ITEM_FUNC(_ThreadUnwaitStatus) 
    SYMBOL_ITEM_FUNC(_ThreadVarInit) 
    SYMBOL_ITEM_FUNC(__timespecToTickDiff) 
    SYMBOL_ITEM_FUNC(__timespecToTickDiff64) 
    SYMBOL_ITEM_FUNC(_TimeCvtInit) 
    SYMBOL_ITEM_FUNC(_TimerInit) 
    SYMBOL_ITEM_FUNC(_ThreadTick) 
    SYMBOL_ITEM_FUNC(_WatchDogTick) 
    SYMBOL_ITEM_FUNC(_WakeupAdd) 
    SYMBOL_ITEM_FUNC(_WakeupDel) 
    SYMBOL_ITEM_FUNC(_WakeupStatus) 
    SYMBOL_ITEM_FUNC(API_CoroutineCreate) 
    SYMBOL_ITEM_FUNC(API_CoroutineDelete) 
    SYMBOL_ITEM_FUNC(API_CoroutineExit) 
    SYMBOL_ITEM_FUNC(API_CoroutineResume) 
    SYMBOL_ITEM_FUNC(API_CoroutineStackCheck) 
    SYMBOL_ITEM_FUNC(API_CoroutineYield) 
    SYMBOL_ITEM_FUNC(API_CpuCurId) 
    SYMBOL_ITEM_FUNC(API_CpuIsRunning) 
    SYMBOL_ITEM_FUNC(API_CpuIsUp) 
    SYMBOL_ITEM_FUNC(API_CpuNum) 
    SYMBOL_ITEM_FUNC(API_CpuPhyId) 
    SYMBOL_ITEM_FUNC(API_CpuUp) 
    SYMBOL_ITEM_FUNC(API_CpuUpNum) 
    SYMBOL_ITEM_FUNC(API_CpuGetSchedAffinity) 
    SYMBOL_ITEM_FUNC(API_CpuSetSchedAffinity) 
    SYMBOL_ITEM_FUNC(_CpuBogoMipsClear) 
    SYMBOL_ITEM_FUNC(API_CpuBogoMips) 
    SYMBOL_ITEM_FUNC(API_CpuPowerGet) 
    SYMBOL_ITEM_FUNC(API_CpuPowerSet) 
    SYMBOL_ITEM_FUNC(API_EventSetCreate) 
    SYMBOL_ITEM_FUNC(API_EventSetDelete) 
    SYMBOL_ITEM_FUNC(API_EventSetGet) 
    SYMBOL_ITEM_FUNC(API_EventSetGetEx) 
    SYMBOL_ITEM_FUNC(API_EventSetGetName) 
    SYMBOL_ITEM_FUNC(API_EventSetSet) 
    SYMBOL_ITEM_FUNC(API_EventSetStatus) 
    SYMBOL_ITEM_FUNC(API_EventSetTryGet) 
    SYMBOL_ITEM_FUNC(API_EventSetTryGetEx) 
    SYMBOL_ITEM_FUNC(API_GetLastError) 
    SYMBOL_ITEM_FUNC(API_GetLastErrorEx) 
    SYMBOL_ITEM_FUNC(API_SetLastError) 
    SYMBOL_ITEM_FUNC(API_SetLastErrorEx) 
    SYMBOL_ITEM_FUNC(API_InterContext) 
    SYMBOL_ITEM_FUNC(API_InterGetNesting) 
    SYMBOL_ITEM_FUNC(API_InterGetNestingById) 
    SYMBOL_ITEM_FUNC(_interDeferInit) 
    SYMBOL_ITEM_FUNC(API_InterDeferContext) 
    SYMBOL_ITEM_FUNC(API_InterDeferGet) 
    SYMBOL_ITEM_FUNC(API_InterDeferJobAdd) 
    SYMBOL_ITEM_FUNC(API_InterDeferJobDelete) 
    SYMBOL_ITEM_FUNC(API_InterEnter) 
    SYMBOL_ITEM_FUNC(API_InterExit) 
    SYMBOL_ITEM_FUNC(API_InterLock) 
    SYMBOL_ITEM_FUNC(API_InterUnlock) 
    SYMBOL_ITEM_FUNC(API_InterVectorGetPriority) 
    SYMBOL_ITEM_FUNC(API_InterVectorSetPriority) 
    SYMBOL_ITEM_FUNC(API_InterStackBaseGet) 
    SYMBOL_ITEM_FUNC(API_InterStackCheck) 
    SYMBOL_ITEM_FUNC(API_InterGetTarget) 
    SYMBOL_ITEM_FUNC(API_InterSetTarget) 
    SYMBOL_ITEM_FUNC(API_InterVectorBaseGet) 
    SYMBOL_ITEM_FUNC(API_InterVectorConnect) 
    SYMBOL_ITEM_FUNC(API_InterVectorConnectEx) 
    SYMBOL_ITEM_FUNC(API_InterVectorDisconnect) 
    SYMBOL_ITEM_FUNC(API_InterVectorDisconnectEx) 
    SYMBOL_ITEM_FUNC(API_InterVectorServiceCnt) 
    SYMBOL_ITEM_FUNC(API_InterVectorDisable) 
    SYMBOL_ITEM_FUNC(API_InterVectorDisableEx) 
    SYMBOL_ITEM_FUNC(API_InterVectorEnable) 
    SYMBOL_ITEM_FUNC(API_InterVectorIsEnable) 
    SYMBOL_ITEM_FUNC(API_InterVectorGetFlag) 
    SYMBOL_ITEM_FUNC(API_InterVectorSetFlag) 
    SYMBOL_ITEM_FUNC(API_InterVectorIpi) 
    SYMBOL_ITEM_FUNC(API_InterVectorIpiEx) 
    SYMBOL_ITEM_FUNC(API_InterVectorIsr) 
    SYMBOL_ITEM_FUNC(API_Atomic64Add) 
    SYMBOL_ITEM_FUNC(API_Atomic64And) 
    SYMBOL_ITEM_FUNC(API_Atomic64Cas) 
    SYMBOL_ITEM_FUNC(API_Atomic64Dec) 
    SYMBOL_ITEM_FUNC(API_Atomic64Get) 
    SYMBOL_ITEM_FUNC(API_Atomic64Inc) 
    SYMBOL_ITEM_FUNC(API_Atomic64Nand) 
    SYMBOL_ITEM_FUNC(API_Atomic64Or) 
    SYMBOL_ITEM_FUNC(API_Atomic64Set) 
    SYMBOL_ITEM_FUNC(API_Atomic64Sub) 
    SYMBOL_ITEM_FUNC(API_Atomic64Swp) 
    SYMBOL_ITEM_FUNC(API_Atomic64Xor) 
    SYMBOL_ITEM_FUNC(API_AtomicAdd) 
    SYMBOL_ITEM_FUNC(API_AtomicAnd) 
    SYMBOL_ITEM_FUNC(API_AtomicCas) 
    SYMBOL_ITEM_FUNC(API_AtomicDec) 
    SYMBOL_ITEM_FUNC(API_AtomicGet) 
    SYMBOL_ITEM_FUNC(API_AtomicInc) 
    SYMBOL_ITEM_FUNC(API_AtomicNand) 
    SYMBOL_ITEM_FUNC(API_AtomicOr) 
    SYMBOL_ITEM_FUNC(API_AtomicSet) 
    SYMBOL_ITEM_FUNC(API_AtomicSub) 
    SYMBOL_ITEM_FUNC(API_AtomicSwp) 
    SYMBOL_ITEM_FUNC(API_AtomicXor) 
    SYMBOL_ITEM_FUNC(API_KernelFpuPrimaryInit) 
    SYMBOL_ITEM_FUNC(API_KernelFpuSecondaryInit) 
    SYMBOL_ITEM_FUNC(API_KernelGetExc) 
    SYMBOL_ITEM_FUNC(API_KernelGetIdle) 
    SYMBOL_ITEM_FUNC(API_KernelGetItimer) 
    SYMBOL_ITEM_FUNC(API_KernelGetPriorityMax) 
    SYMBOL_ITEM_FUNC(API_KernelGetPriorityMin) 
    SYMBOL_ITEM_FUNC(API_KernelGetThreadNum) 
    SYMBOL_ITEM_FUNC(API_KernelGetThreadNumByPriority) 
    SYMBOL_ITEM_FUNC(API_KernelHeapInfo) 
    SYMBOL_ITEM_FUNC(API_KernelHeapInfoEx) 
    SYMBOL_ITEM_FUNC(API_KernelHookGet) 
    SYMBOL_ITEM_FUNC(API_KernelHookSet) 
    SYMBOL_ITEM_FUNC(API_KernelHookDelete) 
    SYMBOL_ITEM_FUNC(API_KernelSmpCall) 
    SYMBOL_ITEM_FUNC(API_KernelSmpCallAll) 
    SYMBOL_ITEM_FUNC(API_KernelSmpCallAllOther) 
    SYMBOL_ITEM_FUNC(API_KernelIsRunning) 
    SYMBOL_ITEM_FUNC(API_KernelIsCpuIdle) 
    SYMBOL_ITEM_FUNC(API_KernelIsSystemIdle) 
    SYMBOL_ITEM_FUNC(API_KernelNop) 
    SYMBOL_ITEM_FUNC(API_ObjectGetClass) 
    SYMBOL_ITEM_FUNC(API_ObjectGetIndex) 
    SYMBOL_ITEM_FUNC(API_ObjectGetNode) 
    SYMBOL_ITEM_FUNC(API_ObjectIsGlobal) 
    SYMBOL_ITEM_FUNC(API_ObjectShareAdd) 
    SYMBOL_ITEM_FUNC(API_ObjectShareDelete) 
    SYMBOL_ITEM_FUNC(API_ObjectShareFind) 
    SYMBOL_ITEM_FUNC(API_KernelStartParam) 
    SYMBOL_ITEM_FUNC(API_KernelStartParamGet) 
    SYMBOL_ITEM_FUNC(API_KernelReboot) 
    SYMBOL_ITEM_FUNC(API_KernelRebootEx) 
    SYMBOL_ITEM_FUNC(API_KernelResume) 
    SYMBOL_ITEM_FUNC(API_SpinDestory) 
    SYMBOL_ITEM_FUNC(API_SpinInit) 
    SYMBOL_ITEM_FUNC(API_SpinLock) 
    SYMBOL_ITEM_FUNC(API_SpinLockIrq) 
    SYMBOL_ITEM_FUNC(API_SpinLockQuick) 
    SYMBOL_ITEM_FUNC(API_SpinRestrict) 
    SYMBOL_ITEM_FUNC(API_SpinTryLock) 
    SYMBOL_ITEM_FUNC(API_SpinTryLockIrq) 
    SYMBOL_ITEM_FUNC(API_SpinUnlock) 
    SYMBOL_ITEM_FUNC(API_SpinUnlockIrq) 
    SYMBOL_ITEM_FUNC(API_SpinUnlockQuick) 
    SYMBOL_ITEM_FUNC(API_KernelPrimaryStart) 
    SYMBOL_ITEM_FUNC(API_KernelSecondaryStart) 
    SYMBOL_ITEM_FUNC(API_KernelSuspend) 
    SYMBOL_ITEM_FUNC(API_KernelTicks) 
    SYMBOL_ITEM_FUNC(API_KernelTicksAdjust) 
    SYMBOL_ITEM_FUNC(API_KernelTicksContext) 
    SYMBOL_ITEM_FUNC(uuidgen) 
    SYMBOL_ITEM_FUNC(API_KernelVerinfo) 
    SYMBOL_ITEM_FUNC(API_KernelVerpatch) 
    SYMBOL_ITEM_FUNC(API_KernelVersion) 
    SYMBOL_ITEM_FUNC(API_MsgQueueClear) 
    SYMBOL_ITEM_FUNC(API_MsgQueueCreate) 
    SYMBOL_ITEM_FUNC(API_MsgQueueDelete) 
    SYMBOL_ITEM_FUNC(API_MsgQueueFlush) 
    SYMBOL_ITEM_FUNC(API_MsgQueueFlushReceive) 
    SYMBOL_ITEM_FUNC(API_MsgQueueFlushSend) 
    SYMBOL_ITEM_FUNC(API_MsgQueueGetName) 
    SYMBOL_ITEM_FUNC(API_MsgQueueReceive) 
    SYMBOL_ITEM_FUNC(API_MsgQueueReceiveEx) 
    SYMBOL_ITEM_FUNC(API_MsgQueueSend) 
    SYMBOL_ITEM_FUNC(API_MsgQueueSend2) 
    SYMBOL_ITEM_FUNC(API_MsgQueueSendEx) 
    SYMBOL_ITEM_FUNC(API_MsgQueueSendEx2) 
    SYMBOL_ITEM_FUNC(API_MsgQueueStatus) 
    SYMBOL_ITEM_FUNC(API_MsgQueueStatusEx) 
    SYMBOL_ITEM_FUNC(API_MsgQueueTryReceive) 
    SYMBOL_ITEM_FUNC(API_PartitionCreate) 
    SYMBOL_ITEM_FUNC(API_PartitionDelete) 
    SYMBOL_ITEM_FUNC(API_PartitionDeleteEx) 
    SYMBOL_ITEM_FUNC(API_PartitionGet) 
    SYMBOL_ITEM_FUNC(API_PartitionGetName) 
    SYMBOL_ITEM_FUNC(API_PartitionPut) 
    SYMBOL_ITEM_FUNC(API_PartitionStatus) 
    SYMBOL_ITEM_FUNC(API_RegionAddMem) 
    SYMBOL_ITEM_FUNC(API_RegionCreate) 
    SYMBOL_ITEM_FUNC(API_RegionDelete) 
    SYMBOL_ITEM_FUNC(API_RegionDeleteEx) 
    SYMBOL_ITEM_FUNC(API_RegionGet) 
    SYMBOL_ITEM_FUNC(API_RegionGetAlign) 
    SYMBOL_ITEM_FUNC(API_RegionGetName) 
    SYMBOL_ITEM_FUNC(API_RegionPut) 
    SYMBOL_ITEM_FUNC(API_RegionReget) 
    SYMBOL_ITEM_FUNC(API_RegionGetMax) 
    SYMBOL_ITEM_FUNC(API_RegionStatus) 
    SYMBOL_ITEM_FUNC(API_RegionStatusEx) 
    SYMBOL_ITEM_FUNC(API_RmsCancel) 
    SYMBOL_ITEM_FUNC(API_RmsCreate) 
    SYMBOL_ITEM_FUNC(API_RmsDelete) 
    SYMBOL_ITEM_FUNC(API_RmsDeleteEx) 
    SYMBOL_ITEM_FUNC(API_RmsExecTimeGet) 
    SYMBOL_ITEM_FUNC(API_RmsGetName) 
    SYMBOL_ITEM_FUNC(API_RmsPeriod) 
    SYMBOL_ITEM_FUNC(API_RmsStatus) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBClear) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBCreate) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBDelete) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBFlush) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPendEx) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPost) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPost2) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPostEx) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBPostEx2) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBRelease) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBStatus) 
    SYMBOL_ITEM_FUNC(API_SemaphoreBTryPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCClear) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCCreate) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCDelete) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCFlush) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCPost) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCRelease) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCStatus) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCStatusEx) 
    SYMBOL_ITEM_FUNC(API_SemaphoreCTryPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreDelete) 
    SYMBOL_ITEM_FUNC(API_SemaphoreFlush) 
    SYMBOL_ITEM_FUNC(API_SemaphoreGetName) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMCreate) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMDelete) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMPost) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMStatus) 
    SYMBOL_ITEM_FUNC(API_SemaphoreMStatusEx) 
    SYMBOL_ITEM_FUNC(API_SemaphorePend) 
    SYMBOL_ITEM_FUNC(API_SemaphorePost) 
    SYMBOL_ITEM_FUNC(API_SemaphorePostBPend) 
    SYMBOL_ITEM_FUNC(API_SemaphorePostCPend) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWCreate) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWDelete) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWPendR) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWPendW) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWPost) 
    SYMBOL_ITEM_FUNC(API_SemaphoreRWStatus) 
    SYMBOL_ITEM_FUNC(API_ThreadGetAffinity) 
    SYMBOL_ITEM_FUNC(API_ThreadSetAffinity) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrBuild) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrBuildDefault) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrBuildEx) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrBuildFP) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrGet) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrGetDefault) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrSetArg) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrSetGuardSize) 
    SYMBOL_ITEM_FUNC(API_ThreadAttrSetStackSize) 
    SYMBOL_ITEM_FUNC(API_ThreadCancel) 
    SYMBOL_ITEM_FUNC(API_ThreadCancelWatchDog) 
    SYMBOL_ITEM_FUNC(API_ThreadCPUUsageRefresh) 
    SYMBOL_ITEM_FUNC(API_ThreadCreate) 
    SYMBOL_ITEM_FUNC(__threadDelete) 
    SYMBOL_ITEM_FUNC(_exit) 
    SYMBOL_ITEM_FUNC(API_ThreadDelete) 
    SYMBOL_ITEM_FUNC(API_ThreadExit) 
    SYMBOL_ITEM_FUNC(atexit) 
    SYMBOL_ITEM_FUNC(exit) 
    SYMBOL_ITEM_FUNC(API_ThreadDesc) 
    SYMBOL_ITEM_FUNC(API_ThreadDetach) 
    SYMBOL_ITEM_FUNC(API_ThreadDetachEx) 
    SYMBOL_ITEM_FUNC(API_ThreadFeedWatchDog) 
    SYMBOL_ITEM_FUNC(API_ThreadForceDelete) 
    SYMBOL_ITEM_FUNC(API_ThreadForceResume) 
    SYMBOL_ITEM_FUNC(API_ThreadCPUUsageIsOn) 
    SYMBOL_ITEM_FUNC(API_ThreadCPUUsageOff) 
    SYMBOL_ITEM_FUNC(API_ThreadCPUUsageOn) 
    SYMBOL_ITEM_FUNC(API_ThreadGetCPUUsage) 
    SYMBOL_ITEM_FUNC(API_ThreadGetCPUUsageAll) 
    SYMBOL_ITEM_FUNC(API_ThreadGetName) 
    SYMBOL_ITEM_FUNC(API_ThreadCurNotePad) 
    SYMBOL_ITEM_FUNC(API_ThreadGetNotePad) 
    SYMBOL_ITEM_FUNC(API_ThreadGetPriority) 
    SYMBOL_ITEM_FUNC(API_ThreadGetSchedParam) 
    SYMBOL_ITEM_FUNC(API_ThreadGetSlice) 
    SYMBOL_ITEM_FUNC(API_ThreadGetSliceEx) 
    SYMBOL_ITEM_FUNC(API_ThreadGetStackMini) 
    SYMBOL_ITEM_FUNC(API_ThreadIdInter) 
    SYMBOL_ITEM_FUNC(API_ThreadIdSelf) 
    SYMBOL_ITEM_FUNC(API_ThreadTcbInter) 
    SYMBOL_ITEM_FUNC(API_ThreadTcbSelf) 
    SYMBOL_ITEM_FUNC(API_ThreadInit) 
    SYMBOL_ITEM_FUNC(API_ThreadIsLock) 
    SYMBOL_ITEM_FUNC(API_ThreadIsReady) 
    SYMBOL_ITEM_FUNC(API_ThreadIsRunning) 
    SYMBOL_ITEM_FUNC(API_ThreadIsSafe) 
    SYMBOL_ITEM_FUNC(API_ThreadIsSuspend) 
    SYMBOL_ITEM_FUNC(API_ThreadJoin) 
    SYMBOL_ITEM_FUNC(API_ThreadLock) 
    SYMBOL_ITEM_FUNC(API_ThreadRestart) 
    SYMBOL_ITEM_FUNC(API_ThreadRestartEx) 
    SYMBOL_ITEM_FUNC(API_ThreadResume) 
    SYMBOL_ITEM_FUNC(API_ThreadSafe) 
    SYMBOL_ITEM_FUNC(API_ThreadSetCancelState) 
    SYMBOL_ITEM_FUNC(API_ThreadSetCancelType) 
    SYMBOL_ITEM_FUNC(API_ThreadSetName) 
    SYMBOL_ITEM_FUNC(API_ThreadSetNotePad) 
    SYMBOL_ITEM_FUNC(API_ThreadSetPriority) 
    SYMBOL_ITEM_FUNC(API_ThreadSetSchedParam) 
    SYMBOL_ITEM_FUNC(API_ThreadSetSlice) 
    SYMBOL_ITEM_FUNC(API_ThreadStackCheck) 
    SYMBOL_ITEM_FUNC(API_ThreadStart) 
    SYMBOL_ITEM_FUNC(API_ThreadStartEx) 
    SYMBOL_ITEM_FUNC(API_ThreadContinue) 
    SYMBOL_ITEM_FUNC(API_ThreadStop) 
    SYMBOL_ITEM_FUNC(API_ThreadSuspend) 
    SYMBOL_ITEM_FUNC(API_ThreadTestCancel) 
    SYMBOL_ITEM_FUNC(API_ThreadUnlock) 
    SYMBOL_ITEM_FUNC(API_ThreadUnsafe) 
    SYMBOL_ITEM_FUNC(API_ThreadVerify) 
    SYMBOL_ITEM_FUNC(API_ThreadWakeup) 
    SYMBOL_ITEM_FUNC(API_ThreadWakeupEx) 
    SYMBOL_ITEM_FUNC(API_ThreadYield) 
    SYMBOL_ITEM_FUNC(API_TimeGet) 
    SYMBOL_ITEM_FUNC(API_TimeGet64) 
    SYMBOL_ITEM_FUNC(API_TimeGetFrequency) 
    SYMBOL_ITEM_FUNC(API_TimeSet) 
    SYMBOL_ITEM_FUNC(API_TimerCancel) 
    SYMBOL_ITEM_FUNC(API_TimerCreate) 
    SYMBOL_ITEM_FUNC(API_TimerDelete) 
    SYMBOL_ITEM_FUNC(API_TimerGetName) 
    SYMBOL_ITEM_FUNC(API_TimerHGetFrequency) 
    SYMBOL_ITEM_FUNC(API_TimerHTicks) 
    SYMBOL_ITEM_FUNC(API_TimerReset) 
    SYMBOL_ITEM_FUNC(API_TimerStart) 
    SYMBOL_ITEM_FUNC(API_TimerStartEx) 
    SYMBOL_ITEM_FUNC(API_TimerStatus) 
    SYMBOL_ITEM_FUNC(API_TimerStatusEx) 
    SYMBOL_ITEM_FUNC(API_TimeMSleep) 
    SYMBOL_ITEM_FUNC(API_TimeNanoSleepMethod) 
    SYMBOL_ITEM_FUNC(API_TimeSleep) 
    SYMBOL_ITEM_FUNC(API_TimeSleepEx) 
    SYMBOL_ITEM_FUNC(API_TimeSleepUntil) 
    SYMBOL_ITEM_FUNC(API_TimeSSleep) 
    SYMBOL_ITEM_FUNC(nanosleep) 
    SYMBOL_ITEM_FUNC(sleep) 
    SYMBOL_ITEM_FUNC(usleep) 
    SYMBOL_ITEM_FUNC(API_TimeTodAdj) 
    SYMBOL_ITEM_FUNC(API_TimeTodAdjEx) 
    SYMBOL_ITEM_FUNC(getegid) 
    SYMBOL_ITEM_FUNC(geteuid) 
    SYMBOL_ITEM_FUNC(getgid) 
    SYMBOL_ITEM_FUNC(getgroups) 
    SYMBOL_ITEM_FUNC(getlogin) 
    SYMBOL_ITEM_FUNC(getlogin_r) 
    SYMBOL_ITEM_FUNC(getuid) 
    SYMBOL_ITEM_FUNC(setegid) 
    SYMBOL_ITEM_FUNC(seteuid) 
    SYMBOL_ITEM_FUNC(setgid) 
    SYMBOL_ITEM_FUNC(setgroups) 
    SYMBOL_ITEM_FUNC(setuid) 
    SYMBOL_ITEM_FUNC(API_WorkQueueCreate) 
    SYMBOL_ITEM_FUNC(API_WorkQueueDelete) 
    SYMBOL_ITEM_FUNC(API_WorkQueueFlush) 
    SYMBOL_ITEM_FUNC(API_WorkQueueInsert) 
    SYMBOL_ITEM_FUNC(API_WorkQueueStatus) 
    SYMBOL_ITEM_FUNC(_Allocate_Event_Object) 
    SYMBOL_ITEM_FUNC(_Free_Event_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_EventSet_Object) 
    SYMBOL_ITEM_FUNC(_Free_EventSet_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_Heap_Object) 
    SYMBOL_ITEM_FUNC(_Free_Heap_Object) 
    SYMBOL_ITEM_FUNC(_List_Line_Add_Ahead) 
    SYMBOL_ITEM_FUNC(_List_Line_Add_Left) 
    SYMBOL_ITEM_FUNC(_List_Line_Add_Right) 
    SYMBOL_ITEM_FUNC(_List_Line_Add_Tail) 
    SYMBOL_ITEM_FUNC(_List_Line_Del) 
    SYMBOL_ITEM_FUNC(_List_Ring_Add_Ahead) 
    SYMBOL_ITEM_FUNC(_List_Ring_Add_Front) 
    SYMBOL_ITEM_FUNC(_List_Ring_Add_Last) 
    SYMBOL_ITEM_FUNC(_List_Ring_Del) 
    SYMBOL_ITEM_FUNC(_Allocate_MsgQueue_Object) 
    SYMBOL_ITEM_FUNC(_Free_MsgQueue_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_Partition_Object) 
    SYMBOL_ITEM_FUNC(_Free_Partition_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_Rms_Object) 
    SYMBOL_ITEM_FUNC(_Free_Rms_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_Tcb_Object) 
    SYMBOL_ITEM_FUNC(_Free_Tcb_Object) 
    SYMBOL_ITEM_FUNC(_Allocate_Timer_Object) 
    SYMBOL_ITEM_FUNC(_Free_Timer_Object) 
    SYMBOL_ITEM_FUNC(__resAddRawHook) 
    SYMBOL_ITEM_FUNC(__resDelRawHook) 
    SYMBOL_ITEM_FUNC(__resHandleIsGlobal) 
    SYMBOL_ITEM_FUNC(__resHandleMakeGlobal) 
    SYMBOL_ITEM_FUNC(__resHandleMakeLocal) 
    SYMBOL_ITEM_FUNC(__resPidReclaim) 
    SYMBOL_ITEM_FUNC(__resPidReclaimOnlyRaw) 
    SYMBOL_ITEM_FUNC(__resThreadDelHook) 
    SYMBOL_ITEM_FUNC(_resInit) 
    SYMBOL_ITEM_FUNC(__resReclaimInit) 
    SYMBOL_ITEM_FUNC(__resReclaimReq) 
    SYMBOL_ITEM_FUNC(API_BacktracePrint) 
    SYMBOL_ITEM_FUNC(API_BacktraceShow) 
    SYMBOL_ITEM_FUNC(API_CPUUsageShow) 
    SYMBOL_ITEM_FUNC(API_EventSetShow) 
    SYMBOL_ITEM_FUNC(API_InterShow) 
    SYMBOL_ITEM_FUNC(API_MsgQueueShow) 
    SYMBOL_ITEM_FUNC(API_PartitionShow) 
    SYMBOL_ITEM_FUNC(API_RegionShow) 
    SYMBOL_ITEM_FUNC(API_RmsShow) 
    SYMBOL_ITEM_FUNC(API_SemaphoreShow) 
    SYMBOL_ITEM_FUNC(API_StackShow) 
    SYMBOL_ITEM_FUNC(API_ThreadPendShow) 
    SYMBOL_ITEM_FUNC(API_ThreadPendShowEx) 
    SYMBOL_ITEM_FUNC(API_ThreadShow) 
    SYMBOL_ITEM_FUNC(API_ThreadShowEx) 
    SYMBOL_ITEM_FUNC(API_ThreadWjShow) 
    SYMBOL_ITEM_FUNC(API_ThreadWjShowEx) 
    SYMBOL_ITEM_FUNC(API_TimerShow) 
    SYMBOL_ITEM_FUNC(API_TimeShow) 
    SYMBOL_ITEM_FUNC(API_VmmAbortShow) 
    SYMBOL_ITEM_FUNC(API_VmmPhysicalShow) 
    SYMBOL_ITEM_FUNC(API_VmmVirtualShow) 
    SYMBOL_ITEM_FUNC(API_ThreadCleanupPop) 
    SYMBOL_ITEM_FUNC(API_ThreadCleanupPush) 
    SYMBOL_ITEM_FUNC(API_ThreadCleanupPushEx) 
    SYMBOL_ITEM_FUNC(API_ThreadCondAttrDestroy) 
    SYMBOL_ITEM_FUNC(API_ThreadCondAttrGetPshared) 
    SYMBOL_ITEM_FUNC(API_ThreadCondAttrInit) 
    SYMBOL_ITEM_FUNC(API_ThreadCondAttrSetPshared) 
    SYMBOL_ITEM_FUNC(API_ThreadCondBroadcast) 
    SYMBOL_ITEM_FUNC(API_ThreadCondDestroy) 
    SYMBOL_ITEM_FUNC(API_ThreadCondInit) 
    SYMBOL_ITEM_FUNC(API_ThreadCondSignal) 
    SYMBOL_ITEM_FUNC(API_ThreadCondWait) 
    SYMBOL_ITEM_FUNC(API_ThreadOnce) 
    SYMBOL_ITEM_FUNC(API_ThreadOnce2) 
    SYMBOL_ITEM_FUNC(_Tree_Rb_Erase) 
    SYMBOL_ITEM_FUNC(_Tree_Rb_Insert_Color) 
    SYMBOL_ITEM_FUNC(__pageAllocate) 
    SYMBOL_ITEM_FUNC(__pageAllocateAlign) 
    SYMBOL_ITEM_FUNC(__pageCbInit) 
    SYMBOL_ITEM_FUNC(__pageExpand) 
    SYMBOL_ITEM_FUNC(__pageFindLink) 
    SYMBOL_ITEM_FUNC(__pageFree) 
    SYMBOL_ITEM_FUNC(__pageGetMinContinue) 
    SYMBOL_ITEM_FUNC(__pageLink) 
    SYMBOL_ITEM_FUNC(__pageMerge) 
    SYMBOL_ITEM_FUNC(__pageSplit) 
    SYMBOL_ITEM_FUNC(__pageTraversalLink) 
    SYMBOL_ITEM_FUNC(__pageUnlink) 
    SYMBOL_ITEM_FUNC(__pageZoneCreate) 
    SYMBOL_ITEM_FUNC(__vmmGetCurCtx) 
    SYMBOL_ITEM_FUNC(__vmmLibFlushTlb) 
    SYMBOL_ITEM_FUNC(__vmmLibGetFlag) 
    SYMBOL_ITEM_FUNC(__vmmLibPageMap) 
    SYMBOL_ITEM_FUNC(__vmmLibPageMap2) 
    SYMBOL_ITEM_FUNC(__vmmLibPrimaryInit) 
    SYMBOL_ITEM_FUNC(__vmmLibSecondaryInit) 
    SYMBOL_ITEM_FUNC(__vmmLibSetFlag) 
    SYMBOL_ITEM_FUNC(__vmmLibVirtualOverlap) 
    SYMBOL_ITEM_FUNC(__vmmLibVirtualToPhysical) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalCreate) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalGetKernelDesc) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalGetZone) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageAlloc) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageAllocAlign) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageAllocZone) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageClear) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageClearAll) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageClone) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFaultCheck) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFaultClear) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFaultGuarder) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFaultLimit) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFlush) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFlushAll) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFree) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageFreeAll) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageGetMinContinue) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageInvalidate) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageInvalidateAll) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageRef) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageSetFlag) 
    SYMBOL_ITEM_FUNC(__vmmPhysicalPageSetFlagAll) 
    SYMBOL_ITEM_FUNC(__vmmVirDevPageAlloc) 
    SYMBOL_ITEM_FUNC(__vmmVirDevPageAllocAlign) 
    SYMBOL_ITEM_FUNC(__vmmVirDevPageFree) 
    SYMBOL_ITEM_FUNC(__vmmVirtualCreate) 
    SYMBOL_ITEM_FUNC(__vmmVirtualDesc) 
    SYMBOL_ITEM_FUNC(__vmmVirtualIsInside) 
    SYMBOL_ITEM_FUNC(__vmmVirtualPageAlloc) 
    SYMBOL_ITEM_FUNC(__vmmVirtualPageAllocAlign) 
    SYMBOL_ITEM_FUNC(__vmmVirtualPageFree) 
    SYMBOL_ITEM_FUNC(__vmmVirtualSwitch) 
    SYMBOL_ITEM_FUNC(API_VmmDmaAlloc) 
    SYMBOL_ITEM_FUNC(API_VmmDmaAllocAlign) 
    SYMBOL_ITEM_FUNC(API_VmmDmaAllocAlignWithFlags) 
    SYMBOL_ITEM_FUNC(API_VmmDmaFree) 
    SYMBOL_ITEM_FUNC(API_VmmGetFlag) 
    SYMBOL_ITEM_FUNC(API_VmmGetLibBlock) 
    SYMBOL_ITEM_FUNC(API_VmmLibAddPhyRam) 
    SYMBOL_ITEM_FUNC(API_VmmLibPrimaryInit) 
    SYMBOL_ITEM_FUNC(API_VmmLibSecondaryInit) 
    SYMBOL_ITEM_FUNC(API_VmmMap) 
    SYMBOL_ITEM_FUNC(API_VmmMmuDisable) 
    SYMBOL_ITEM_FUNC(API_VmmMmuEnable) 
    SYMBOL_ITEM_FUNC(API_VmmPageFaultGuarder) 
    SYMBOL_ITEM_FUNC(API_VmmPageFaultLimit) 
    SYMBOL_ITEM_FUNC(API_VmmPhyAlloc) 
    SYMBOL_ITEM_FUNC(API_VmmPhyAllocAlign) 
    SYMBOL_ITEM_FUNC(API_VmmPhyAllocEx) 
    SYMBOL_ITEM_FUNC(API_VmmPhyFree) 
    SYMBOL_ITEM_FUNC(API_VmmPhysicalKernelDesc) 
    SYMBOL_ITEM_FUNC(API_VmmPhysicalToVirtual) 
    SYMBOL_ITEM_FUNC(API_VmmSetFlag) 
    SYMBOL_ITEM_FUNC(API_VmmVirtualIsInside) 
    SYMBOL_ITEM_FUNC(API_VmmVirtualStatus) 
    SYMBOL_ITEM_FUNC(API_VmmVirtualToPhysical) 
    SYMBOL_ITEM_FUNC(API_VmmZoneStatus) 
    SYMBOL_ITEM_FUNC(getpagesize) 
    SYMBOL_ITEM_FUNC(__vmmAbortFakeSymbol) 
    SYMBOL_ITEM_FUNC(API_VmmAbortIsr) 
    SYMBOL_ITEM_FUNC(API_VmmAbortIsrEx) 
    SYMBOL_ITEM_FUNC(API_VmmAbortReturn) 
    SYMBOL_ITEM_FUNC(API_VmmAbortStatus) 
    SYMBOL_ITEM_FUNC(__areaPhysicalInsertPage) 
    SYMBOL_ITEM_FUNC(__areaPhysicalSearchPage) 
    SYMBOL_ITEM_FUNC(__areaPhysicalSpaceInit) 
    SYMBOL_ITEM_FUNC(__areaPhysicalSpaceTraversal) 
    SYMBOL_ITEM_FUNC(__areaPhysicalUnlinkPage) 
    SYMBOL_ITEM_FUNC(__areaVirtualInsertPage) 
    SYMBOL_ITEM_FUNC(__areaVirtualSearchPage) 
    SYMBOL_ITEM_FUNC(__areaVirtualSpaceInit) 
    SYMBOL_ITEM_FUNC(__areaVirtualSpaceTraversal) 
    SYMBOL_ITEM_FUNC(__areaVirtualUnlinkPage) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemap) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemap2) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemapEx) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemapEx2) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemapNocache) 
    SYMBOL_ITEM_FUNC(API_VmmIoRemapNocache2) 
    SYMBOL_ITEM_FUNC(API_VmmIoUnmap) 
    SYMBOL_ITEM_FUNC(API_VmmExpandArea) 
    SYMBOL_ITEM_FUNC(API_VmmFree) 
    SYMBOL_ITEM_FUNC(API_VmmFreeArea) 
    SYMBOL_ITEM_FUNC(API_VmmInvalidateArea) 
    SYMBOL_ITEM_FUNC(API_VmmMalloc) 
    SYMBOL_ITEM_FUNC(API_VmmMallocAlign) 
    SYMBOL_ITEM_FUNC(API_VmmMallocArea) 
    SYMBOL_ITEM_FUNC(API_VmmMallocAreaAlign) 
    SYMBOL_ITEM_FUNC(API_VmmMallocAreaEx) 
    SYMBOL_ITEM_FUNC(API_VmmMallocEx) 
    SYMBOL_ITEM_FUNC(API_VmmMergeArea) 
    SYMBOL_ITEM_FUNC(API_VmmMoveArea) 
    SYMBOL_ITEM_FUNC(API_VmmPCountInArea) 
    SYMBOL_ITEM_FUNC(API_VmmPreallocArea) 
    SYMBOL_ITEM_FUNC(API_VmmRemapArea) 
    SYMBOL_ITEM_FUNC(API_VmmRemapArea2) 
    SYMBOL_ITEM_FUNC(API_VmmSetFiller) 
    SYMBOL_ITEM_FUNC(API_VmmSetFindShare) 
    SYMBOL_ITEM_FUNC(API_VmmShareArea) 
    SYMBOL_ITEM_FUNC(API_VmmSplitArea) 
    SYMBOL_ITEM_FUNC(__vmmMapInit) 
    SYMBOL_ITEM_FUNC(API_VmmMmap) 
    SYMBOL_ITEM_FUNC(API_VmmMmapPCount) 
    SYMBOL_ITEM_FUNC(API_VmmMmapReclaim) 
    SYMBOL_ITEM_FUNC(API_VmmMmapShow) 
    SYMBOL_ITEM_FUNC(API_VmmMProtect) 
    SYMBOL_ITEM_FUNC(API_VmmMremap) 
    SYMBOL_ITEM_FUNC(API_VmmMsync) 
    SYMBOL_ITEM_FUNC(API_VmmMunmap) 
    SYMBOL_ITEM_FUNC(API_VmmStackAlloc) 
    SYMBOL_ITEM_FUNC(API_VmmStackFree) 
    SYMBOL_ITEM_FUNC(__vmmPageSwapIsNeedLoad) 
    SYMBOL_ITEM_FUNC(__vmmPageSwapLoad) 
    SYMBOL_ITEM_FUNC(__vmmPageSwapSwitch) 
    SYMBOL_ITEM_FUNC(__assert) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(abs) 
    SYMBOL_ITEM_FUNC(aligned_free) 
    SYMBOL_ITEM_FUNC(aligned_malloc) 
    SYMBOL_ITEM_FUNC(asctime) 
    SYMBOL_ITEM_FUNC(asctime_r) 
    SYMBOL_ITEM_FUNC(atof) 
    SYMBOL_ITEM_FUNC(atoi) 
    SYMBOL_ITEM_FUNC(atol) 
    SYMBOL_ITEM_FUNC(atoll) 
    SYMBOL_ITEM_FUNC(bcmp) 
    SYMBOL_ITEM_FUNC(bcopy) 
    SYMBOL_ITEM_FUNC(brk) 
    SYMBOL_ITEM_FUNC(bsearch) 
    SYMBOL_ITEM_FUNC(bzero) 
    SYMBOL_ITEM_FUNC(calloc) 
    SYMBOL_ITEM_FUNC(clock) 
    SYMBOL_ITEM_FUNC(clock_getcpuclockid) 
    SYMBOL_ITEM_FUNC(clock_getres) 
    SYMBOL_ITEM_FUNC(clock_gettime) 
    SYMBOL_ITEM_FUNC(clock_nanosleep) 
    SYMBOL_ITEM_FUNC(clock_settime) 
    SYMBOL_ITEM_FUNC(ctime) 
    SYMBOL_ITEM_FUNC(ctime_r) 
    SYMBOL_ITEM_FUNC(difftime) 
    SYMBOL_ITEM_FUNC(div) 
    SYMBOL_ITEM_FUNC(drand48) 
    SYMBOL_ITEM_FUNC(erand48) 
    SYMBOL_ITEM_FUNC(ffs) 
    SYMBOL_ITEM_FUNC(ffsll) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(getenv) 
    SYMBOL_ITEM_FUNC(getenv_r) 
    SYMBOL_ITEM_FUNC(gethrtime) 
    SYMBOL_ITEM_FUNC(gethrvtime) 
    SYMBOL_ITEM_FUNC(gettimeofday) 
    SYMBOL_ITEM_FUNC(gmtime) 
    SYMBOL_ITEM_FUNC(gmtime_r) 
    SYMBOL_ITEM_FUNC(imaxabs) 
    SYMBOL_ITEM_FUNC(imaxdiv) 
    SYMBOL_ITEM_FUNC(index) 
    SYMBOL_ITEM_FUNC(isalnum) 
    SYMBOL_ITEM_FUNC(isalpha) 
    SYMBOL_ITEM_FUNC(isascii) 
    SYMBOL_ITEM_FUNC(isblank) 
    SYMBOL_ITEM_FUNC(iscntrl) 
    SYMBOL_ITEM_FUNC(isdigit) 
    SYMBOL_ITEM_FUNC(isgraph) 
    SYMBOL_ITEM_FUNC(isinf) 
    SYMBOL_ITEM_FUNC(islower) 
    SYMBOL_ITEM_FUNC(isnan) 
    SYMBOL_ITEM_FUNC(isprint) 
    SYMBOL_ITEM_FUNC(ispunct) 
    SYMBOL_ITEM_FUNC(isspace) 
    SYMBOL_ITEM_FUNC(isupper) 
    SYMBOL_ITEM_FUNC(isxdigit) 
    SYMBOL_ITEM_FUNC(itoa) 
    SYMBOL_ITEM_FUNC(jrand48) 
    SYMBOL_ITEM_FUNC(labs) 
    SYMBOL_ITEM_FUNC(lcong48) 
    SYMBOL_ITEM_FUNC(ldiv) 
    SYMBOL_ITEM_FUNC(llabs) 
    SYMBOL_ITEM_FUNC(lldiv) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(localtime) 
    SYMBOL_ITEM_FUNC(localtime_r) 
    SYMBOL_ITEM_FUNC(lrand48) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(malloc_new) 
    SYMBOL_ITEM_FUNC(mallocalign) 
    SYMBOL_ITEM_FUNC(memalign) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcmp) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(mempcpy) 
    SYMBOL_ITEM_FUNC(memrchr) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(mktime) 
    SYMBOL_ITEM_FUNC(mrand48) 
    SYMBOL_ITEM_FUNC(nrand48) 
    SYMBOL_ITEM_FUNC(posix_memalign) 
    SYMBOL_ITEM_FUNC(putenv) 
    SYMBOL_ITEM_FUNC(qsort) 
    SYMBOL_ITEM_FUNC(rand) 
    SYMBOL_ITEM_FUNC(rand_r) 
    SYMBOL_ITEM_FUNC(random) 
    SYMBOL_ITEM_FUNC(realloc) 
    SYMBOL_ITEM_FUNC(rindex) 
    SYMBOL_ITEM_FUNC(sbrk) 
    SYMBOL_ITEM_FUNC(seed48) 
    SYMBOL_ITEM_FUNC(setenv) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(settimeofday) 
    SYMBOL_ITEM_FUNC(srand) 
    SYMBOL_ITEM_FUNC(srand48) 
    SYMBOL_ITEM_FUNC(srandom) 
    SYMBOL_ITEM_FUNC(stpcpy) 
    SYMBOL_ITEM_FUNC(stpncpy) 
    SYMBOL_ITEM_FUNC(strcasecmp) 
    SYMBOL_ITEM_FUNC(strcasestr) 
    SYMBOL_ITEM_FUNC(strcat) 
    SYMBOL_ITEM_FUNC(strchr) 
    SYMBOL_ITEM_FUNC(strchrnul) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strcoll) 
    SYMBOL_ITEM_FUNC(strcpy) 
    SYMBOL_ITEM_FUNC(strcspn) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(strerror) 
    SYMBOL_ITEM_FUNC(strerror_r) 
    SYMBOL_ITEM_FUNC(strftime) 
    SYMBOL_ITEM_FUNC(stricmp) 
    SYMBOL_ITEM_FUNC(strlcat) 
    SYMBOL_ITEM_FUNC(strlcpy) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strncasecmp) 
    SYMBOL_ITEM_FUNC(strncat) 
    SYMBOL_ITEM_FUNC(strncmp) 
    SYMBOL_ITEM_FUNC(strncpy) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(strnlen) 
    SYMBOL_ITEM_FUNC(strnset) 
    SYMBOL_ITEM_FUNC(strpbrk) 
    SYMBOL_ITEM_FUNC(strptime) 
    SYMBOL_ITEM_FUNC(strrchr) 
    SYMBOL_ITEM_FUNC(strsep) 
    SYMBOL_ITEM_FUNC(strsignal) 
    SYMBOL_ITEM_FUNC(strspn) 
    SYMBOL_ITEM_FUNC(strstr) 
    SYMBOL_ITEM_FUNC(strtod) 
    SYMBOL_ITEM_FUNC(strtof) 
    SYMBOL_ITEM_FUNC(strtoimax) 
    SYMBOL_ITEM_FUNC(strtok) 
    SYMBOL_ITEM_FUNC(strtok_r) 
    SYMBOL_ITEM_FUNC(strtol) 
    SYMBOL_ITEM_FUNC(strtold) 
    SYMBOL_ITEM_FUNC(strtoll) 
    SYMBOL_ITEM_FUNC(strtoul) 
    SYMBOL_ITEM_FUNC(strtoull) 
    SYMBOL_ITEM_FUNC(strtoumax) 
    SYMBOL_ITEM_FUNC(strxfrm) 
    SYMBOL_ITEM_FUNC(swab) 
    SYMBOL_ITEM_FUNC(system) 
    SYMBOL_ITEM_FUNC(timegm) 
    SYMBOL_ITEM_FUNC(timelocal) 
    SYMBOL_ITEM_FUNC(toascii) 
    SYMBOL_ITEM_FUNC(tolower) 
    SYMBOL_ITEM_FUNC(toupper) 
    SYMBOL_ITEM_FUNC(tzset) 
    SYMBOL_ITEM_FUNC(unsetenv) 
    SYMBOL_ITEM_FUNC(wcstoimax) 
    SYMBOL_ITEM_FUNC(wcstoumax) 
    SYMBOL_ITEM_FUNC(xcalloc) 
    SYMBOL_ITEM_FUNC(xmalloc) 
    SYMBOL_ITEM_FUNC(xmallocalign) 
    SYMBOL_ITEM_FUNC(xmemalign) 
    SYMBOL_ITEM_FUNC(xrealloc) 
    SYMBOL_ITEM_FUNC(xstrdup) 
    SYMBOL_ITEM_FUNC(xstrndup) 
    SYMBOL_ITEM_FUNC(__bcrypt) 
    SYMBOL_ITEM_FUNC(__gensalt_blowfish) 
    SYMBOL_ITEM_FUNC(bcrypt_gensalt) 
    SYMBOL_ITEM_FUNC(__crypt_sha1) 
    SYMBOL_ITEM_FUNC(__crypt_sha1_iterations) 
    SYMBOL_ITEM_FUNC(crypt) 
    SYMBOL_ITEM_FUNC(crypt_safe) 
    SYMBOL_ITEM_FUNC(des_cipher) 
    SYMBOL_ITEM_FUNC(des_setkey) 
    SYMBOL_ITEM_FUNC(encrypt) 
    SYMBOL_ITEM_FUNC(setkey) 
    SYMBOL_ITEM_FUNC(__hmac_sha1) 
    SYMBOL_ITEM_FUNC(__md5crypt) 
    SYMBOL_ITEM_FUNC(__gensalt_md5) 
    SYMBOL_ITEM_FUNC(__gensalt_new) 
    SYMBOL_ITEM_FUNC(__gensalt_old) 
    SYMBOL_ITEM_FUNC(__gensalt_sha1) 
    SYMBOL_ITEM_FUNC(pw_gensalt) 
    SYMBOL_ITEM_FUNC(__crypt_to64) 
    SYMBOL_ITEM_FUNC(lib_isalnum) 
    SYMBOL_ITEM_FUNC(lib_isalpha) 
    SYMBOL_ITEM_FUNC(lib_isascii) 
    SYMBOL_ITEM_FUNC(lib_isblank) 
    SYMBOL_ITEM_FUNC(lib_iscntrl) 
    SYMBOL_ITEM_FUNC(lib_isdigit) 
    SYMBOL_ITEM_FUNC(lib_isgraph) 
    SYMBOL_ITEM_FUNC(lib_islower) 
    SYMBOL_ITEM_FUNC(lib_isprint) 
    SYMBOL_ITEM_FUNC(lib_ispunct) 
    SYMBOL_ITEM_FUNC(lib_isspace) 
    SYMBOL_ITEM_FUNC(lib_isupper) 
    SYMBOL_ITEM_FUNC(lib_isxdigit) 
    SYMBOL_ITEM_FUNC(lib_toascii) 
    SYMBOL_ITEM_FUNC(panic) 
    SYMBOL_ITEM_FUNC(lib_isinf) 
    SYMBOL_ITEM_FUNC(lib_isnan) 
    SYMBOL_ITEM_FUNC(lib_imaxabs) 
    SYMBOL_ITEM_FUNC(lib_imaxdiv) 
    SYMBOL_ITEM_FUNC(lib_strtoimax) 
    SYMBOL_ITEM_FUNC(lib_strtoumax) 
    SYMBOL_ITEM_FUNC(lib_wcstoimax) 
    SYMBOL_ITEM_FUNC(lib_wcstoumax) 
    SYMBOL_ITEM_FUNC(__longjmpSetup) 
    SYMBOL_ITEM_FUNC(__setjmpSetup) 
    SYMBOL_ITEM_FUNC(asprintf) 
    SYMBOL_ITEM_FUNC(vasprintf) 
    SYMBOL_ITEM_FUNC(clearerr) 
    SYMBOL_ITEM_FUNC(ctermid) 
    SYMBOL_ITEM_FUNC(lib_cvtfloat) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(fclose_ex) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(fdprintf) 
    SYMBOL_ITEM_FUNC(fdscanf) 
    SYMBOL_ITEM_FUNC(feof) 
    SYMBOL_ITEM_FUNC(ferror) 
    SYMBOL_ITEM_FUNC(__sflush) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(fgetc) 
    SYMBOL_ITEM_FUNC(__slbexpand) 
    SYMBOL_ITEM_FUNC(fgetln) 
    SYMBOL_ITEM_FUNC(fgetpos) 
    SYMBOL_ITEM_FUNC(fgetpos64) 
    SYMBOL_ITEM_FUNC(fgets) 
    SYMBOL_ITEM_FUNC(fileno) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(fopen) 
    SYMBOL_ITEM_FUNC(fopen64) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(fpurge) 
    SYMBOL_ITEM_FUNC(fputc) 
    SYMBOL_ITEM_FUNC(fputs) 
    SYMBOL_ITEM_FUNC(fread) 
    SYMBOL_ITEM_FUNC(freopen) 
    SYMBOL_ITEM_FUNC(fscanf) 
    SYMBOL_ITEM_FUNC(fseek) 
    SYMBOL_ITEM_FUNC(fseeko) 
    SYMBOL_ITEM_FUNC(fseeko64) 
    SYMBOL_ITEM_FUNC(fsetpos) 
    SYMBOL_ITEM_FUNC(fsetpos64) 
    SYMBOL_ITEM_FUNC(ftell) 
    SYMBOL_ITEM_FUNC(ftello) 
    SYMBOL_ITEM_FUNC(ftello64) 
    SYMBOL_ITEM_FUNC(funopen) 
    SYMBOL_ITEM_FUNC(__sfvwrite) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(fwrite) 
    SYMBOL_ITEM_FUNC(getc) 
    SYMBOL_ITEM_FUNC(getchar) 
    SYMBOL_ITEM_FUNC(gets) 
    SYMBOL_ITEM_FUNC(getw) 
    SYMBOL_ITEM_FUNC(__lib_delfile) 
    SYMBOL_ITEM_FUNC(__lib_newfile) 
    SYMBOL_ITEM_FUNC(__lib_stderr) 
    SYMBOL_ITEM_FUNC(__lib_stdin) 
    SYMBOL_ITEM_FUNC(__lib_stdout) 
    SYMBOL_ITEM_FUNC(__smakebuf) 
    SYMBOL_ITEM_FUNC(__swhatbuf) 
    SYMBOL_ITEM_FUNC(_mktemp) 
    SYMBOL_ITEM_FUNC(mkdtemp) 
    SYMBOL_ITEM_FUNC(mkstemp) 
    SYMBOL_ITEM_FUNC(mkstemps) 
    SYMBOL_ITEM_FUNC(mktemp) 
    SYMBOL_ITEM_FUNC(perror) 
    SYMBOL_ITEM_FUNC(pclose) 
    SYMBOL_ITEM_FUNC(popen) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(putc) 
    SYMBOL_ITEM_FUNC(putchar) 
    SYMBOL_ITEM_FUNC(puts) 
    SYMBOL_ITEM_FUNC(putw) 
    SYMBOL_ITEM_FUNC(__srefill) 
    SYMBOL_ITEM_FUNC(remove) 
    SYMBOL_ITEM_FUNC(rewind) 
    SYMBOL_ITEM_FUNC(__srget) 
    SYMBOL_ITEM_FUNC(scanf) 
    SYMBOL_ITEM_FUNC(setbuf) 
    SYMBOL_ITEM_FUNC(setbuffer) 
    SYMBOL_ITEM_FUNC(setlinebuf) 
    SYMBOL_ITEM_FUNC(setvbuf) 
    SYMBOL_ITEM_FUNC(bnprintf) 
    SYMBOL_ITEM_FUNC(snprintf) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(sscanf) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(tempnam) 
    SYMBOL_ITEM_FUNC(tmpfile) 
    SYMBOL_ITEM_FUNC(tmpnam) 
    SYMBOL_ITEM_FUNC(tmpnam_r) 
    SYMBOL_ITEM_FUNC(ungetc) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(__svfscanf) 
    SYMBOL_ITEM_FUNC(vfscanf) 
    SYMBOL_ITEM_FUNC(vprintf) 
    SYMBOL_ITEM_FUNC(vscanf) 
    SYMBOL_ITEM_FUNC(vbnprintf) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(vsprintf) 
    SYMBOL_ITEM_FUNC(vsscanf) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swsetup) 
    SYMBOL_ITEM_FUNC(lib_abs) 
    SYMBOL_ITEM_FUNC(lib_labs) 
    SYMBOL_ITEM_FUNC(lib_llabs) 
    SYMBOL_ITEM_FUNC(lib_getenv) 
    SYMBOL_ITEM_FUNC(lib_getenv_r) 
    SYMBOL_ITEM_FUNC(lib_putenv) 
    SYMBOL_ITEM_FUNC(lib_setenv) 
    SYMBOL_ITEM_FUNC(lib_unsetenv) 
    SYMBOL_ITEM_FUNC(lib_div) 
    SYMBOL_ITEM_FUNC(lib_ldiv) 
    SYMBOL_ITEM_FUNC(lib_lldiv) 
    SYMBOL_ITEM_FUNC(lib_calloc) 
    SYMBOL_ITEM_FUNC(lib_free) 
    SYMBOL_ITEM_FUNC(lib_malloc) 
    SYMBOL_ITEM_FUNC(lib_malloc_new) 
    SYMBOL_ITEM_FUNC(lib_mallocalign) 
    SYMBOL_ITEM_FUNC(lib_memalign) 
    SYMBOL_ITEM_FUNC(lib_posix_memalign) 
    SYMBOL_ITEM_FUNC(lib_realloc) 
    SYMBOL_ITEM_FUNC(lib_xcalloc) 
    SYMBOL_ITEM_FUNC(lib_xmalloc) 
    SYMBOL_ITEM_FUNC(lib_xmallocalign) 
    SYMBOL_ITEM_FUNC(lib_xmemalign) 
    SYMBOL_ITEM_FUNC(lib_xrealloc) 
    SYMBOL_ITEM_FUNC(lib_drand48) 
    SYMBOL_ITEM_FUNC(lib_erand48) 
    SYMBOL_ITEM_FUNC(lib_jrand48) 
    SYMBOL_ITEM_FUNC(lib_lcong48) 
    SYMBOL_ITEM_FUNC(lib_lrand48) 
    SYMBOL_ITEM_FUNC(lib_mrand48) 
    SYMBOL_ITEM_FUNC(lib_nrand48) 
    SYMBOL_ITEM_FUNC(lib_rand) 
    SYMBOL_ITEM_FUNC(lib_rand_r) 
    SYMBOL_ITEM_FUNC(lib_random) 
    SYMBOL_ITEM_FUNC(lib_seed48) 
    SYMBOL_ITEM_FUNC(lib_srand) 
    SYMBOL_ITEM_FUNC(lib_srand48) 
    SYMBOL_ITEM_FUNC(lib_srandom) 
    SYMBOL_ITEM_FUNC(lib_bsearch) 
    SYMBOL_ITEM_FUNC(lib_qsort) 
    SYMBOL_ITEM_FUNC(lib_atof) 
    SYMBOL_ITEM_FUNC(lib_atoi) 
    SYMBOL_ITEM_FUNC(lib_atol) 
    SYMBOL_ITEM_FUNC(lib_atoll) 
    SYMBOL_ITEM_FUNC(lib_itoa) 
    SYMBOL_ITEM_FUNC(lib_strtof) 
    SYMBOL_ITEM_FUNC(lib_strtol) 
    SYMBOL_ITEM_FUNC(lib_strtoll) 
    SYMBOL_ITEM_FUNC(lib_strtoul) 
    SYMBOL_ITEM_FUNC(lib_strtoull) 
    SYMBOL_ITEM_FUNC(lib_strtod) 
    SYMBOL_ITEM_FUNC(lib_strtold) 
    SYMBOL_ITEM_FUNC(lib_abort) 
    SYMBOL_ITEM_FUNC(lib_system) 
    SYMBOL_ITEM_FUNC(lib_ffs) 
    SYMBOL_ITEM_FUNC(lib_ffsll) 
    SYMBOL_ITEM_FUNC(lib_index) 
    SYMBOL_ITEM_FUNC(lib_memchr) 
    SYMBOL_ITEM_FUNC(lib_bcmp) 
    SYMBOL_ITEM_FUNC(lib_memcmp) 
    SYMBOL_ITEM_FUNC(lib_memcpy) 
    SYMBOL_ITEM_FUNC(lib_mempcpy) 
    SYMBOL_ITEM_FUNC(lib_memrchr) 
    SYMBOL_ITEM_FUNC(lib_bzero) 
    SYMBOL_ITEM_FUNC(lib_memset) 
    SYMBOL_ITEM_FUNC(lib_rindex) 
    SYMBOL_ITEM_FUNC(lib_stpcpy) 
    SYMBOL_ITEM_FUNC(lib_stpncpy) 
    SYMBOL_ITEM_FUNC(lib_strcasecmp) 
    SYMBOL_ITEM_FUNC(lib_strcasestr) 
    SYMBOL_ITEM_FUNC(lib_strcat) 
    SYMBOL_ITEM_FUNC(lib_strchrnul) 
    SYMBOL_ITEM_FUNC(lib_strcmp) 
    SYMBOL_ITEM_FUNC(lib_strcpy) 
    SYMBOL_ITEM_FUNC(lib_strcspn) 
    SYMBOL_ITEM_FUNC(lib_strdup) 
    SYMBOL_ITEM_FUNC(lib_strerror) 
    SYMBOL_ITEM_FUNC(lib_strerror_r) 
    SYMBOL_ITEM_FUNC(lib_strftime) 
    SYMBOL_ITEM_FUNC(lib_stricmp) 
    SYMBOL_ITEM_FUNC(lib_strlen) 
    SYMBOL_ITEM_FUNC(lib_strncasecmp) 
    SYMBOL_ITEM_FUNC(lib_strlcat) 
    SYMBOL_ITEM_FUNC(lib_strncat) 
    SYMBOL_ITEM_FUNC(lib_strncmp) 
    SYMBOL_ITEM_FUNC(lib_bcopy) 
    SYMBOL_ITEM_FUNC(lib_strlcpy) 
    SYMBOL_ITEM_FUNC(lib_strncpy) 
    SYMBOL_ITEM_FUNC(lib_strndup) 
    SYMBOL_ITEM_FUNC(lib_strnlen) 
    SYMBOL_ITEM_FUNC(lib_strnset) 
    SYMBOL_ITEM_FUNC(lib_strpbrk) 
    SYMBOL_ITEM_FUNC(lib_strptime) 
    SYMBOL_ITEM_FUNC(lib_strsep) 
    SYMBOL_ITEM_FUNC(lib_strsignal) 
    SYMBOL_ITEM_FUNC(lib_strspn) 
    SYMBOL_ITEM_FUNC(lib_strstr) 
    SYMBOL_ITEM_FUNC(lib_strtok) 
    SYMBOL_ITEM_FUNC(lib_strtok_r) 
    SYMBOL_ITEM_FUNC(lib_strxfrm) 
    SYMBOL_ITEM_FUNC(lib_swab) 
    SYMBOL_ITEM_FUNC(lib_tolower) 
    SYMBOL_ITEM_FUNC(lib_toupper) 
    SYMBOL_ITEM_FUNC(lib_xstrdup) 
    SYMBOL_ITEM_FUNC(lib_xstrndup) 
    SYMBOL_ITEM_FUNC(fstatvfs) 
    SYMBOL_ITEM_FUNC(statvfs) 
    SYMBOL_ITEM_FUNC(lib_asctime) 
    SYMBOL_ITEM_FUNC(lib_asctime_r) 
    SYMBOL_ITEM_FUNC(lib_clock) 
    SYMBOL_ITEM_FUNC(lib_clock_getcpuclockid) 
    SYMBOL_ITEM_FUNC(lib_clock_getres) 
    SYMBOL_ITEM_FUNC(lib_clock_gettime) 
    SYMBOL_ITEM_FUNC(lib_clock_nanosleep) 
    SYMBOL_ITEM_FUNC(lib_clock_settime) 
    SYMBOL_ITEM_FUNC(lib_ctime) 
    SYMBOL_ITEM_FUNC(lib_ctime_r) 
    SYMBOL_ITEM_FUNC(lib_gettimeofday) 
    SYMBOL_ITEM_FUNC(lib_settimeofday) 
    SYMBOL_ITEM_FUNC(lib_difftime) 
    SYMBOL_ITEM_FUNC(__daysSinceEpoch) 
    SYMBOL_ITEM_FUNC(__gettime) 
    SYMBOL_ITEM_FUNC(__julday) 
    SYMBOL_ITEM_FUNC(lib_gmtime) 
    SYMBOL_ITEM_FUNC(lib_gmtime_r) 
    SYMBOL_ITEM_FUNC(lib_gethrtime) 
    SYMBOL_ITEM_FUNC(lib_gethrvtime) 
    SYMBOL_ITEM_FUNC(lib_localtime) 
    SYMBOL_ITEM_FUNC(lib_localtime_r) 
    SYMBOL_ITEM_FUNC(lib_mktime) 
    SYMBOL_ITEM_FUNC(lib_timegm) 
    SYMBOL_ITEM_FUNC(lib_time) 
    SYMBOL_ITEM_FUNC(lib_timelocal) 
    SYMBOL_ITEM_FUNC(time) 
    SYMBOL_ITEM_FUNC(lib_tzset) 
    SYMBOL_ITEM_FUNC(endgrent) 
    SYMBOL_ITEM_FUNC(endpwent) 
    SYMBOL_ITEM_FUNC(getgrent) 
    SYMBOL_ITEM_FUNC(getgrgid) 
    SYMBOL_ITEM_FUNC(getgrgid_r) 
    SYMBOL_ITEM_FUNC(getgrnam) 
    SYMBOL_ITEM_FUNC(getgrnam_r) 
    SYMBOL_ITEM_FUNC(getpwent) 
    SYMBOL_ITEM_FUNC(getpwnam) 
    SYMBOL_ITEM_FUNC(getpwnam_r) 
    SYMBOL_ITEM_FUNC(getpwuid) 
    SYMBOL_ITEM_FUNC(getpwuid_r) 
    SYMBOL_ITEM_FUNC(initgroups) 
    SYMBOL_ITEM_FUNC(scangr) 
    SYMBOL_ITEM_FUNC(scanInt) 
    SYMBOL_ITEM_FUNC(scanpw) 
    SYMBOL_ITEM_FUNC(scanString) 
    SYMBOL_ITEM_FUNC(setgrent) 
    SYMBOL_ITEM_FUNC(setpwent) 
    SYMBOL_ITEM_FUNC(endspent) 
    SYMBOL_ITEM_FUNC(fgetspent) 
    SYMBOL_ITEM_FUNC(fgetspent_r) 
    SYMBOL_ITEM_FUNC(getpass) 
    SYMBOL_ITEM_FUNC(getpass_r) 
    SYMBOL_ITEM_FUNC(getspent) 
    SYMBOL_ITEM_FUNC(getspent_r) 
    SYMBOL_ITEM_FUNC(getspnam) 
    SYMBOL_ITEM_FUNC(getspnam_r) 
    SYMBOL_ITEM_FUNC(passwdcheck) 
    SYMBOL_ITEM_FUNC(putspent) 
    SYMBOL_ITEM_FUNC(scansp) 
    SYMBOL_ITEM_FUNC(setspent) 
    SYMBOL_ITEM_FUNC(userlogin) 
    SYMBOL_ITEM_FUNC(user_db_gadd) 
    SYMBOL_ITEM_FUNC(user_db_gdel) 
    SYMBOL_ITEM_FUNC(user_db_gget) 
    SYMBOL_ITEM_FUNC(user_db_pmod) 
    SYMBOL_ITEM_FUNC(user_db_uadd) 
    SYMBOL_ITEM_FUNC(user_db_udel) 
    SYMBOL_ITEM_FUNC(user_db_uget) 
    SYMBOL_ITEM_FUNC(user_db_umod) 
    SYMBOL_ITEM_FUNC(btowc) 
    SYMBOL_ITEM_FUNC(fgetwc) 
    SYMBOL_ITEM_FUNC(fgetws) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(fputws) 
    SYMBOL_ITEM_FUNC(fwide) 
    SYMBOL_ITEM_FUNC(fwprintf) 
    SYMBOL_ITEM_FUNC(fwscanf) 
    SYMBOL_ITEM_FUNC(getwc) 
    SYMBOL_ITEM_FUNC(getwchar) 
    SYMBOL_ITEM_FUNC(iswalnum) 
    SYMBOL_ITEM_FUNC(iswalpha) 
    SYMBOL_ITEM_FUNC(iswcntrl) 
    SYMBOL_ITEM_FUNC(iswctype) 
    SYMBOL_ITEM_FUNC(iswdigit) 
    SYMBOL_ITEM_FUNC(iswgraph) 
    SYMBOL_ITEM_FUNC(iswlower) 
    SYMBOL_ITEM_FUNC(iswprint) 
    SYMBOL_ITEM_FUNC(iswpunct) 
    SYMBOL_ITEM_FUNC(iswspace) 
    SYMBOL_ITEM_FUNC(iswupper) 
    SYMBOL_ITEM_FUNC(iswxdigit) 
    SYMBOL_ITEM_FUNC(mblen) 
    SYMBOL_ITEM_FUNC(mbrlen) 
    SYMBOL_ITEM_FUNC(mbrtowc) 
    SYMBOL_ITEM_FUNC(mbsinit) 
    SYMBOL_ITEM_FUNC(mbsrtowcs) 
    SYMBOL_ITEM_FUNC(mbstowcs) 
    SYMBOL_ITEM_FUNC(mbtowc) 
    SYMBOL_ITEM_FUNC(putwc) 
    SYMBOL_ITEM_FUNC(putwchar) 
    SYMBOL_ITEM_FUNC(swprintf) 
    SYMBOL_ITEM_FUNC(swscanf) 
    SYMBOL_ITEM_FUNC(towlower) 
    SYMBOL_ITEM_FUNC(towupper) 
    SYMBOL_ITEM_FUNC(ungetwc) 
    SYMBOL_ITEM_FUNC(vfwprintf) 
    SYMBOL_ITEM_FUNC(vfwscanf) 
    SYMBOL_ITEM_FUNC(vswprintf) 
    SYMBOL_ITEM_FUNC(vswscanf) 
    SYMBOL_ITEM_FUNC(vwprintf) 
    SYMBOL_ITEM_FUNC(vwscanf) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(wcscoll) 
    SYMBOL_ITEM_FUNC(wcsftime) 
    SYMBOL_ITEM_FUNC(wcsrtombs) 
    SYMBOL_ITEM_FUNC(wcstod) 
    SYMBOL_ITEM_FUNC(wcstof) 
    SYMBOL_ITEM_FUNC(wcstol) 
    SYMBOL_ITEM_FUNC(wcstold) 
    SYMBOL_ITEM_FUNC(wcstoll) 
    SYMBOL_ITEM_FUNC(wcstombs) 
    SYMBOL_ITEM_FUNC(wcstoul) 
    SYMBOL_ITEM_FUNC(wcstoull) 
    SYMBOL_ITEM_FUNC(wcswidth) 
    SYMBOL_ITEM_FUNC(wcsxfrm) 
    SYMBOL_ITEM_FUNC(wctob) 
    SYMBOL_ITEM_FUNC(wctomb) 
    SYMBOL_ITEM_FUNC(wctype) 
    SYMBOL_ITEM_FUNC(wcwidth) 
    SYMBOL_ITEM_FUNC(wprintf) 
    SYMBOL_ITEM_FUNC(wscanf) 
    SYMBOL_ITEM_FUNC(wcscasecmp) 
    SYMBOL_ITEM_FUNC(wcscat) 
    SYMBOL_ITEM_FUNC(wcschr) 
    SYMBOL_ITEM_FUNC(wcscmp) 
    SYMBOL_ITEM_FUNC(wcscpy) 
    SYMBOL_ITEM_FUNC(wcscspn) 
    SYMBOL_ITEM_FUNC(wcsdup) 
    SYMBOL_ITEM_FUNC(wcslcat) 
    SYMBOL_ITEM_FUNC(wcslcpy) 
    SYMBOL_ITEM_FUNC(wcslen) 
    SYMBOL_ITEM_FUNC(wcsncasecmp) 
    SYMBOL_ITEM_FUNC(wcsncat) 
    SYMBOL_ITEM_FUNC(wcsncmp) 
    SYMBOL_ITEM_FUNC(wcsncpy) 
    SYMBOL_ITEM_FUNC(wcspbrk) 
    SYMBOL_ITEM_FUNC(wcsrchr) 
    SYMBOL_ITEM_FUNC(wcsspn) 
    SYMBOL_ITEM_FUNC(wcsstr) 
    SYMBOL_ITEM_FUNC(wcstok) 
    SYMBOL_ITEM_FUNC(wcswcs) 
    SYMBOL_ITEM_FUNC(wmemchr) 
    SYMBOL_ITEM_FUNC(wmemcmp) 
    SYMBOL_ITEM_FUNC(wmemcpy) 
    SYMBOL_ITEM_FUNC(wmemmove) 
    SYMBOL_ITEM_FUNC(wmemset) 
    SYMBOL_ITEM_FUNC(__sync_synchronize) 
    SYMBOL_ITEM_FUNC(__getreent) 
    SYMBOL_ITEM_FUNC(lib_nlreent_init) 
    SYMBOL_ITEM_FUNC(lib_nlreent_static) 
    SYMBOL_ITEM_FUNC(lib_nlreent_stdfile) 
    SYMBOL_ITEM_FUNC(__elfListLoad) 
    SYMBOL_ITEM_FUNC(__elfListUnload) 
    SYMBOL_ITEM_FUNC(__elfStatus) 
    SYMBOL_ITEM_FUNC(API_ModuleFinish) 
    SYMBOL_ITEM_FUNC(API_ModuleGcov) 
    SYMBOL_ITEM_FUNC(API_ModuleGetName) 
    SYMBOL_ITEM_FUNC(API_ModuleGlobal) 
    SYMBOL_ITEM_FUNC(API_ModuleLoad) 
    SYMBOL_ITEM_FUNC(API_ModuleLoadEx) 
    SYMBOL_ITEM_FUNC(API_ModuleProcSym) 
    SYMBOL_ITEM_FUNC(API_ModuleShareConfig) 
    SYMBOL_ITEM_FUNC(API_ModuleShareRefresh) 
    SYMBOL_ITEM_FUNC(API_ModuleStatus) 
    SYMBOL_ITEM_FUNC(API_ModuleSym) 
    SYMBOL_ITEM_FUNC(API_ModuleTerminal) 
    SYMBOL_ITEM_FUNC(API_ModuleUnload) 
    SYMBOL_ITEM_FUNC(moduleLoadSub) 
    SYMBOL_ITEM_FUNC(vprocGetAffinity) 
    SYMBOL_ITEM_FUNC(vprocSetAffinity) 
    SYMBOL_ITEM_FUNC(__processStart) 
    SYMBOL_ITEM_FUNC(__spawnArgActionDup) 
    SYMBOL_ITEM_FUNC(__spawnArgCreate) 
    SYMBOL_ITEM_FUNC(__spawnArgFree) 
    SYMBOL_ITEM_FUNC(__spawnArgProc) 
    SYMBOL_ITEM_FUNC(execl) 
    SYMBOL_ITEM_FUNC(execle) 
    SYMBOL_ITEM_FUNC(execlp) 
    SYMBOL_ITEM_FUNC(execv) 
    SYMBOL_ITEM_FUNC(execve) 
    SYMBOL_ITEM_FUNC(execvp) 
    SYMBOL_ITEM_FUNC(execvpe) 
    SYMBOL_ITEM_FUNC(spawnl) 
    SYMBOL_ITEM_FUNC(spawnle) 
    SYMBOL_ITEM_FUNC(spawnlp) 
    SYMBOL_ITEM_FUNC(spawnv) 
    SYMBOL_ITEM_FUNC(spawnve) 
    SYMBOL_ITEM_FUNC(spawnvp) 
    SYMBOL_ITEM_FUNC(spawnvpe) 
    SYMBOL_ITEM_FUNC(vprocIoFileDeinit) 
    SYMBOL_ITEM_FUNC(vprocIoFileDescGet) 
    SYMBOL_ITEM_FUNC(vprocIoFileDup) 
    SYMBOL_ITEM_FUNC(vprocIoFileDup2) 
    SYMBOL_ITEM_FUNC(vprocIoFileDupFrom) 
    SYMBOL_ITEM_FUNC(vprocIoFileGet) 
    SYMBOL_ITEM_FUNC(vprocIoFileGetEx) 
    SYMBOL_ITEM_FUNC(vprocIoFileInit) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefDec) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefDecArryByPid) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefDecByPid) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefGet) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefGetByPid) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefInc) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefIncArryByPid) 
    SYMBOL_ITEM_FUNC(vprocIoFileRefIncByPid) 
    SYMBOL_ITEM_FUNC(vprocIoReclaim) 
    SYMBOL_ITEM_FUNC(__ldFree) 
    SYMBOL_ITEM_FUNC(__ldFreeArea) 
    SYMBOL_ITEM_FUNC(__ldMalloc) 
    SYMBOL_ITEM_FUNC(__ldMallocAlign) 
    SYMBOL_ITEM_FUNC(__ldMallocArea) 
    SYMBOL_ITEM_FUNC(__ldMallocAreaAlign) 
    SYMBOL_ITEM_FUNC(__ldMmap) 
    SYMBOL_ITEM_FUNC(__ldProtect) 
    SYMBOL_ITEM_FUNC(__ldShare) 
    SYMBOL_ITEM_FUNC(__ldShareAbort) 
    SYMBOL_ITEM_FUNC(__ldShareConfig) 
    SYMBOL_ITEM_FUNC(vprocProcAdd) 
    SYMBOL_ITEM_FUNC(vprocProcDelete) 
    SYMBOL_ITEM_FUNC(__ldGetFilePath) 
    SYMBOL_ITEM_FUNC(__ldPathIsFile) 
    SYMBOL_ITEM_FUNC(API_LoaderInit) 
    SYMBOL_ITEM_FUNC(__moduleDeleteAllSymbol) 
    SYMBOL_ITEM_FUNC(__moduleExportSymbol) 
    SYMBOL_ITEM_FUNC(__moduleFindKernelSymHook) 
    SYMBOL_ITEM_FUNC(__moduleFindSym) 
    SYMBOL_ITEM_FUNC(__moduleSymbolBufferSize) 
    SYMBOL_ITEM_FUNC(__moduleSymGetValue) 
    SYMBOL_ITEM_FUNC(__moduleTraverseKernelSymHook) 
    SYMBOL_ITEM_FUNC(__moduleTraverseSym) 
    SYMBOL_ITEM_FUNC(__moduleVerifyVersion) 
    SYMBOL_ITEM_FUNC(vprocDebugContinue) 
    SYMBOL_ITEM_FUNC(vprocDebugStop) 
    SYMBOL_ITEM_FUNC(vprocDebugThreadContinue) 
    SYMBOL_ITEM_FUNC(vprocDebugThreadGet) 
    SYMBOL_ITEM_FUNC(vprocDebugThreadStop) 
    SYMBOL_ITEM_FUNC(__moduleVpPatchFini) 
    SYMBOL_ITEM_FUNC(__moduleVpPatchInit) 
    SYMBOL_ITEM_FUNC(__moduleVpPatchVersion) 
    SYMBOL_ITEM_FUNC(__moduleVpPatchVmem) 
    SYMBOL_ITEM_FUNC(API_ModuleAddr) 
    SYMBOL_ITEM_FUNC(API_ModuleGetBase) 
    SYMBOL_ITEM_FUNC(API_ModulePid) 
    SYMBOL_ITEM_FUNC(API_ModuleRun) 
    SYMBOL_ITEM_FUNC(API_ModuleRunEx) 
    SYMBOL_ITEM_FUNC(API_ModuleTimes) 
    SYMBOL_ITEM_FUNC(vprocCreate) 
    SYMBOL_ITEM_FUNC(vprocDebugFlagsGet) 
    SYMBOL_ITEM_FUNC(vprocDebugFlagsSet) 
    SYMBOL_ITEM_FUNC(vprocDestroy) 
    SYMBOL_ITEM_FUNC(vprocDetach) 
    SYMBOL_ITEM_FUNC(vprocExit) 
    SYMBOL_ITEM_FUNC(vprocExitModeGet) 
    SYMBOL_ITEM_FUNC(vprocExitModeSet) 
    SYMBOL_ITEM_FUNC(vprocExitNotDestroy) 
    SYMBOL_ITEM_FUNC(vprocFindProc) 
    SYMBOL_ITEM_FUNC(vprocGet) 
    SYMBOL_ITEM_FUNC(vprocGetCur) 
    SYMBOL_ITEM_FUNC(vprocGetFather) 
    SYMBOL_ITEM_FUNC(vprocGetGroup) 
    SYMBOL_ITEM_FUNC(vprocGetMain) 
    SYMBOL_ITEM_FUNC(vprocGetModsInfo) 
    SYMBOL_ITEM_FUNC(vprocGetPath) 
    SYMBOL_ITEM_FUNC(vprocGetPidByTcb) 
    SYMBOL_ITEM_FUNC(vprocGetPidByTcbNoLock) 
    SYMBOL_ITEM_FUNC(vprocGetPidByThread) 
    SYMBOL_ITEM_FUNC(vprocIoEnvGet) 
    SYMBOL_ITEM_FUNC(vprocIsMainThread) 
    SYMBOL_ITEM_FUNC(vprocKillPrepare) 
    SYMBOL_ITEM_FUNC(vprocMainThread) 
    SYMBOL_ITEM_FUNC(vprocNotifyParent) 
    SYMBOL_ITEM_FUNC(vprocReclaim) 
    SYMBOL_ITEM_FUNC(vprocRun) 
    SYMBOL_ITEM_FUNC(vprocSetCur) 
    SYMBOL_ITEM_FUNC(vprocSetGroup) 
    SYMBOL_ITEM_FUNC(vprocSetImmediatelyTerm) 
    SYMBOL_ITEM_FUNC(vprocThreadExitHook) 
    SYMBOL_ITEM_FUNC(vprocTickHook) 
    SYMBOL_ITEM_FUNC(vprocStackAlloc) 
    SYMBOL_ITEM_FUNC(vprocStackFree) 
    SYMBOL_ITEM_FUNC(vprocListGet) 
    SYMBOL_ITEM_FUNC(vprocMemInfo) 
    SYMBOL_ITEM_FUNC(vprocMemInfoNoLock) 
    SYMBOL_ITEM_FUNC(vprocThreadAdd) 
    SYMBOL_ITEM_FUNC(vprocThreadAffinity) 
    SYMBOL_ITEM_FUNC(vprocThreadDelete) 
    SYMBOL_ITEM_FUNC(vprocThreadKill) 
    SYMBOL_ITEM_FUNC(vprocThreadNum) 
    SYMBOL_ITEM_FUNC(vprocThreadSigaction) 
    SYMBOL_ITEM_FUNC(vprocThreadTraversal) 
    SYMBOL_ITEM_FUNC(vprocGetitimer) 
    SYMBOL_ITEM_FUNC(vprocItimerEachHook) 
    SYMBOL_ITEM_FUNC(vprocItimerMainHook) 
    SYMBOL_ITEM_FUNC(vprocSetitimer) 
    SYMBOL_ITEM_FUNC(daemon) 
    SYMBOL_ITEM_FUNC(detach) 
    SYMBOL_ITEM_FUNC(fork) 
    SYMBOL_ITEM_FUNC(getpgid) 
    SYMBOL_ITEM_FUNC(getpgrp) 
    SYMBOL_ITEM_FUNC(getpid) 
    SYMBOL_ITEM_FUNC(getppid) 
    SYMBOL_ITEM_FUNC(isbrother) 
    SYMBOL_ITEM_FUNC(ischild) 
    SYMBOL_ITEM_FUNC(issetugid) 
    SYMBOL_ITEM_FUNC(reclaimchild) 
    SYMBOL_ITEM_FUNC(setpgid) 
    SYMBOL_ITEM_FUNC(setpgrp) 
    SYMBOL_ITEM_FUNC(setsid) 
    SYMBOL_ITEM_FUNC(wait) 
    SYMBOL_ITEM_FUNC(wait3) 
    SYMBOL_ITEM_FUNC(wait4) 
    SYMBOL_ITEM_FUNC(waitid) 
    SYMBOL_ITEM_FUNC(waitpid) 
    SYMBOL_ITEM_FUNC(API_PortCreate) 
    SYMBOL_ITEM_FUNC(API_PortDelete) 
    SYMBOL_ITEM_FUNC(API_PortExToIn) 
    SYMBOL_ITEM_FUNC(API_PortGetName) 
    SYMBOL_ITEM_FUNC(API_PortInToEx) 
    SYMBOL_ITEM_FUNC(_Allocate_Dpma_Object) 
    SYMBOL_ITEM_FUNC(_DpmaInit) 
    SYMBOL_ITEM_FUNC(_Free_Dpma_Object) 
    SYMBOL_ITEM_FUNC(_mpiInit) 
    SYMBOL_ITEM_FUNC(_endhosthtent) 
    SYMBOL_ITEM_FUNC(_gethostbyhtaddr) 
    SYMBOL_ITEM_FUNC(_gethostbyhtname) 
    SYMBOL_ITEM_FUNC(_map_v4v6_address) 
    SYMBOL_ITEM_FUNC(_sethosthtent) 
    SYMBOL_ITEM_FUNC(endhostent) 
    SYMBOL_ITEM_FUNC(gethostent) 
    SYMBOL_ITEM_FUNC(gethostent_r) 
    SYMBOL_ITEM_FUNC(gethostent2_r) 
    SYMBOL_ITEM_FUNC(sethostent) 
    SYMBOL_ITEM_FUNC(freeifaddrs) 
    SYMBOL_ITEM_FUNC(getifaddrs) 
    SYMBOL_ITEM_FUNC(getprotobynumber) 
    SYMBOL_ITEM_FUNC(getprotobynumber_r) 
    SYMBOL_ITEM_FUNC(getprotobyname_static) 
    SYMBOL_ITEM_FUNC(getprotobynumber_static) 
    SYMBOL_ITEM_FUNC(endprotoent) 
    SYMBOL_ITEM_FUNC(getprotoent) 
    SYMBOL_ITEM_FUNC(getprotoent_r) 
    SYMBOL_ITEM_FUNC(setprotoent) 
    SYMBOL_ITEM_FUNC(getprotobyname) 
    SYMBOL_ITEM_FUNC(getprotobyname_r) 
    SYMBOL_ITEM_FUNC(getservbyname) 
    SYMBOL_ITEM_FUNC(getservbyname_r) 
    SYMBOL_ITEM_FUNC(getservbyport) 
    SYMBOL_ITEM_FUNC(getservbyport_r) 
    SYMBOL_ITEM_FUNC(endservent) 
    SYMBOL_ITEM_FUNC(getservent) 
    SYMBOL_ITEM_FUNC(getservent_r) 
    SYMBOL_ITEM_FUNC(setservent) 
    SYMBOL_ITEM_FUNC(inet_ntop) 
    SYMBOL_ITEM_FUNC(inet_pton) 
    SYMBOL_ITEM_FUNC(__ifIoctlArp) 
    SYMBOL_ITEM_FUNC(_netBondingInit) 
    SYMBOL_ITEM_FUNC(_netBridgeInit) 
    SYMBOL_ITEM_FUNC(htonl) 
    SYMBOL_ITEM_FUNC(htons) 
    SYMBOL_ITEM_FUNC(inet_addr) 
    SYMBOL_ITEM_FUNC(inet_aton) 
    SYMBOL_ITEM_FUNC(inet_ntoa) 
    SYMBOL_ITEM_FUNC(inet_ntoa_r) 
    SYMBOL_ITEM_FUNC(inet6_aton) 
    SYMBOL_ITEM_FUNC(inet6_ntoa) 
    SYMBOL_ITEM_FUNC(inet6_ntoa_r) 
    SYMBOL_ITEM_FUNC(ip_gw_hook) 
    SYMBOL_ITEM_FUNC(ip_input_hook) 
    SYMBOL_ITEM_FUNC(ip_route_default_hook) 
    SYMBOL_ITEM_FUNC(ip_route_src_hook) 
    SYMBOL_ITEM_FUNC(ip6_gw_hook) 
    SYMBOL_ITEM_FUNC(ip6_input_hook) 
    SYMBOL_ITEM_FUNC(ip6_route_default_hook) 
    SYMBOL_ITEM_FUNC(ip6_route_src_hook) 
    SYMBOL_ITEM_FUNC(link_input_hook) 
    SYMBOL_ITEM_FUNC(link_output_hook) 
    SYMBOL_ITEM_FUNC(ntohl) 
    SYMBOL_ITEM_FUNC(ntohs) 
    SYMBOL_ITEM_FUNC(sio_open) 
    SYMBOL_ITEM_FUNC(sio_read) 
    SYMBOL_ITEM_FUNC(sio_read_abort) 
    SYMBOL_ITEM_FUNC(sio_recv) 
    SYMBOL_ITEM_FUNC(sio_send) 
    SYMBOL_ITEM_FUNC(sio_tryread) 
    SYMBOL_ITEM_FUNC(sio_write) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_fetch) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_tryfetch) 
    SYMBOL_ITEM_FUNC(sys_arch_msleep) 
    SYMBOL_ITEM_FUNC(sys_arch_protect) 
    SYMBOL_ITEM_FUNC(sys_arch_sem_wait) 
    SYMBOL_ITEM_FUNC(sys_arch_unprotect) 
    SYMBOL_ITEM_FUNC(sys_assert_print) 
    SYMBOL_ITEM_FUNC(sys_error_print) 
    SYMBOL_ITEM_FUNC(sys_init) 
    SYMBOL_ITEM_FUNC(sys_jiffies) 
    SYMBOL_ITEM_FUNC(sys_mbox_free) 
    SYMBOL_ITEM_FUNC(sys_mbox_new) 
    SYMBOL_ITEM_FUNC(sys_mbox_post) 
    SYMBOL_ITEM_FUNC(sys_mbox_post_prio) 
    SYMBOL_ITEM_FUNC(sys_mbox_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost_fromisr) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost_prio) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost_stat) 
    SYMBOL_ITEM_FUNC(sys_mbox_valid) 
    SYMBOL_ITEM_FUNC(sys_mutex_free) 
    SYMBOL_ITEM_FUNC(sys_mutex_lock) 
    SYMBOL_ITEM_FUNC(sys_mutex_new) 
    SYMBOL_ITEM_FUNC(sys_mutex_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mutex_unlock) 
    SYMBOL_ITEM_FUNC(sys_mutex_valid) 
    SYMBOL_ITEM_FUNC(sys_now) 
    SYMBOL_ITEM_FUNC(sys_sem_free) 
    SYMBOL_ITEM_FUNC(sys_sem_new) 
    SYMBOL_ITEM_FUNC(sys_sem_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_sem_signal) 
    SYMBOL_ITEM_FUNC(sys_sem_valid) 
    SYMBOL_ITEM_FUNC(sys_thread_hostent) 
    SYMBOL_ITEM_FUNC(sys_thread_new) 
    SYMBOL_ITEM_FUNC(sys_thread_sem_get) 
    SYMBOL_ITEM_FUNC(__fcIoctlInet) 
    SYMBOL_ITEM_FUNC(__tshellFlowctlInit) 
    SYMBOL_ITEM_FUNC(lwip_ip_hook) 
    SYMBOL_ITEM_FUNC(lwip_ip_nat_hook) 
    SYMBOL_ITEM_FUNC(lwip_ip_route_hook) 
    SYMBOL_ITEM_FUNC(net_ip_hook_add) 
    SYMBOL_ITEM_FUNC(net_ip_hook_delete) 
    SYMBOL_ITEM_FUNC(net_ip_hook_isadd) 
    SYMBOL_ITEM_FUNC(net_ip_hook_nat_add) 
    SYMBOL_ITEM_FUNC(net_ip_hook_nat_delete) 
    SYMBOL_ITEM_FUNC(net_ip_hook_nat_isadd) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_flags) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_gw) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_hwaddr) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_index) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_ip6addr) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_ipaddr) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_linkspeed) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_name) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_netdev) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_netmask) 
    SYMBOL_ITEM_FUNC(net_ip_hook_netif_get_type) 
    SYMBOL_ITEM_FUNC(net_ip_hook_pbuf_set_ifout) 
    SYMBOL_ITEM_FUNC(net_ip_hook_route_add) 
    SYMBOL_ITEM_FUNC(net_ip_hook_route_delete) 
    SYMBOL_ITEM_FUNC(net_ip_hook_route_isadd) 
    SYMBOL_ITEM_FUNC(if_down) 
    SYMBOL_ITEM_FUNC(if_freenameindex) 
    SYMBOL_ITEM_FUNC(if_get_dhcp) 
    SYMBOL_ITEM_FUNC(if_get_dhcp6) 
    SYMBOL_ITEM_FUNC(if_indextoname) 
    SYMBOL_ITEM_FUNC(if_islink) 
    SYMBOL_ITEM_FUNC(if_isup) 
    SYMBOL_ITEM_FUNC(if_list_init) 
    SYMBOL_ITEM_FUNC(if_list_lock) 
    SYMBOL_ITEM_FUNC(if_list_unlock) 
    SYMBOL_ITEM_FUNC(if_nameindex) 
    SYMBOL_ITEM_FUNC(if_nameindex_bufsize) 
    SYMBOL_ITEM_FUNC(if_nameindex_rnp) 
    SYMBOL_ITEM_FUNC(if_nametoindex) 
    SYMBOL_ITEM_FUNC(if_set_dhcp) 
    SYMBOL_ITEM_FUNC(if_set_dhcp6) 
    SYMBOL_ITEM_FUNC(if_up) 
    SYMBOL_ITEM_FUNC(__ifIoctlInet) 
    SYMBOL_ITEM_FUNC(__ifIoctlLp) 
    SYMBOL_ITEM_FUNC(__ifIoctlPacket) 
    SYMBOL_ITEM_FUNC(__ifIoctlPrivate) 
    SYMBOL_ITEM_FUNC(__ifIoctlWireless) 
    SYMBOL_ITEM_FUNC(__ifSubIoctlStats) 
    SYMBOL_ITEM_FUNC(if_param_getaodv) 
    SYMBOL_ITEM_FUNC(if_param_getdefault) 
    SYMBOL_ITEM_FUNC(if_param_getdhcp) 
    SYMBOL_ITEM_FUNC(if_param_getdhcp6) 
    SYMBOL_ITEM_FUNC(if_param_getenable) 
    SYMBOL_ITEM_FUNC(if_param_getgw) 
    SYMBOL_ITEM_FUNC(if_param_getgw_6) 
    SYMBOL_ITEM_FUNC(if_param_getinaddr) 
    SYMBOL_ITEM_FUNC(if_param_getingw) 
    SYMBOL_ITEM_FUNC(if_param_getinnetmask) 
    SYMBOL_ITEM_FUNC(if_param_getipaddr) 
    SYMBOL_ITEM_FUNC(if_param_getipaddr_6) 
    SYMBOL_ITEM_FUNC(if_param_getmac) 
    SYMBOL_ITEM_FUNC(if_param_getmgw) 
    SYMBOL_ITEM_FUNC(if_param_getmipaddr) 
    SYMBOL_ITEM_FUNC(if_param_getmnetmask) 
    SYMBOL_ITEM_FUNC(if_param_getnetmask) 
    SYMBOL_ITEM_FUNC(if_param_gettxqblock) 
    SYMBOL_ITEM_FUNC(if_param_gettxqueue) 
    SYMBOL_ITEM_FUNC(if_param_ipv6autocfg) 
    SYMBOL_ITEM_FUNC(if_param_load) 
    SYMBOL_ITEM_FUNC(if_param_syncdns) 
    SYMBOL_ITEM_FUNC(if_param_tcpackfreq) 
    SYMBOL_ITEM_FUNC(if_param_tcpwnd) 
    SYMBOL_ITEM_FUNC(if_param_unload) 
    SYMBOL_ITEM_FUNC(_netJobqueueInit) 
    SYMBOL_ITEM_FUNC(API_NetJobAdd) 
    SYMBOL_ITEM_FUNC(API_NetJobAddEx) 
    SYMBOL_ITEM_FUNC(API_NetJobDelete) 
    SYMBOL_ITEM_FUNC(API_NetJobDeleteEx) 
    SYMBOL_ITEM_FUNC(API_NetJobGetLost) 
    SYMBOL_ITEM_FUNC(API_LoginBlAdd) 
    SYMBOL_ITEM_FUNC(API_LoginBlDelete) 
    SYMBOL_ITEM_FUNC(API_LoginBlShow) 
    SYMBOL_ITEM_FUNC(API_LoginWlAdd) 
    SYMBOL_ITEM_FUNC(API_LoginWlDelete) 
    SYMBOL_ITEM_FUNC(API_LoginWlShow) 
    SYMBOL_ITEM_FUNC(loginbl_input_hook) 
    SYMBOL_ITEM_FUNC(__mrtIoctlInet) 
    SYMBOL_ITEM_FUNC(netif_callback_init) 
    SYMBOL_ITEM_FUNC(netif_get_flags) 
    SYMBOL_ITEM_FUNC(netif_get_priv_flags) 
    SYMBOL_ITEM_FUNC(netif_set_maddr_hook) 
    SYMBOL_ITEM_FUNC(netif_set_maddr6_hook) 
    SYMBOL_ITEM_FUNC(__tshellNetstatGroup) 
    SYMBOL_ITEM_FUNC(__tshellNetstatIf) 
    SYMBOL_ITEM_FUNC(__tshellNetstatKeepalive) 
    SYMBOL_ITEM_FUNC(__tshellNetstatPacket) 
    SYMBOL_ITEM_FUNC(__tshellNetstatRaw) 
    SYMBOL_ITEM_FUNC(__tshellNetstatStat) 
    SYMBOL_ITEM_FUNC(__tshellNetstatTcp) 
    SYMBOL_ITEM_FUNC(__tshellNetstatTcpListen) 
    SYMBOL_ITEM_FUNC(__tshellNetstatUdp) 
    SYMBOL_ITEM_FUNC(__tshellNetstatUnix) 
    SYMBOL_ITEM_FUNC(__qosIoctlInet) 
    SYMBOL_ITEM_FUNC(__tshellRoute) 
    SYMBOL_ITEM_FUNC(__tshellRouteInit) 
    SYMBOL_ITEM_FUNC(__tshellSrouteInit) 
    SYMBOL_ITEM_FUNC(route_add) 
    SYMBOL_ITEM_FUNC(route_change) 
    SYMBOL_ITEM_FUNC(route_delete) 
    SYMBOL_ITEM_FUNC(route_get) 
    SYMBOL_ITEM_FUNC(route_list) 
    SYMBOL_ITEM_FUNC(__rtIoctlInet) 
    SYMBOL_ITEM_FUNC(__tshellNetInit) 
    SYMBOL_ITEM_FUNC(__tshellNet6Init) 
    SYMBOL_ITEM_FUNC(__socketEnotify) 
    SYMBOL_ITEM_FUNC(__socketEnotify2) 
    SYMBOL_ITEM_FUNC(__socketInit) 
    SYMBOL_ITEM_FUNC(__socketReset) 
    SYMBOL_ITEM_FUNC(accept) 
    SYMBOL_ITEM_FUNC(accept4) 
    SYMBOL_ITEM_FUNC(bind) 
    SYMBOL_ITEM_FUNC(connect) 
    SYMBOL_ITEM_FUNC(freeaddrinfo) 
    SYMBOL_ITEM_FUNC(get_dns_server_info_4) 
    SYMBOL_ITEM_FUNC(get_dns_server_info_6) 
    SYMBOL_ITEM_FUNC(getaddrinfo) 
    SYMBOL_ITEM_FUNC(gethostbyaddr) 
    SYMBOL_ITEM_FUNC(gethostbyaddr_r) 
    SYMBOL_ITEM_FUNC(gethostbyname) 
    SYMBOL_ITEM_FUNC(gethostbyname_r) 
    SYMBOL_ITEM_FUNC(getpeername) 
    SYMBOL_ITEM_FUNC(getsockname) 
    SYMBOL_ITEM_FUNC(getsockopt) 
    SYMBOL_ITEM_FUNC(listen) 
    SYMBOL_ITEM_FUNC(recv) 
    SYMBOL_ITEM_FUNC(recvfrom) 
    SYMBOL_ITEM_FUNC(recvmsg) 
    SYMBOL_ITEM_FUNC(send) 
    SYMBOL_ITEM_FUNC(sendmsg) 
    SYMBOL_ITEM_FUNC(sendto) 
    SYMBOL_ITEM_FUNC(setsockopt) 
    SYMBOL_ITEM_FUNC(shutdown) 
    SYMBOL_ITEM_FUNC(socket) 
    SYMBOL_ITEM_FUNC(socketpair) 
    SYMBOL_ITEM_FUNC(__srtIoctlInet) 
    SYMBOL_ITEM_FUNC(API_NetInit) 
    SYMBOL_ITEM_FUNC(API_NetSnmpInit) 
    SYMBOL_ITEM_FUNC(__ifIoctlVlan) 
    SYMBOL_ITEM_FUNC(_netVndInit) 
    SYMBOL_ITEM_FUNC(srt_sentry_to_srtentry) 
    SYMBOL_ITEM_FUNC(srt_srtentry_to_sentry) 
    SYMBOL_ITEM_FUNC(srt_add_entry) 
    SYMBOL_ITEM_FUNC(srt_delete_entry) 
    SYMBOL_ITEM_FUNC(srt_find_entry) 
    SYMBOL_ITEM_FUNC(srt_netif_add_hook) 
    SYMBOL_ITEM_FUNC(srt_netif_remove_hook) 
    SYMBOL_ITEM_FUNC(srt_route_default_hook) 
    SYMBOL_ITEM_FUNC(srt_route_search_hook) 
    SYMBOL_ITEM_FUNC(srt_search_entry) 
    SYMBOL_ITEM_FUNC(srt_total_entry) 
    SYMBOL_ITEM_FUNC(srt_traversal_entry) 
    SYMBOL_ITEM_FUNC(srt6_sentry_to_srtentry) 
    SYMBOL_ITEM_FUNC(srt6_srtentry_to_sentry) 
    SYMBOL_ITEM_FUNC(srt6_add_entry) 
    SYMBOL_ITEM_FUNC(srt6_delete_entry) 
    SYMBOL_ITEM_FUNC(srt6_find_entry) 
    SYMBOL_ITEM_FUNC(srt6_netif_add_hook) 
    SYMBOL_ITEM_FUNC(srt6_netif_remove_hook) 
    SYMBOL_ITEM_FUNC(srt6_route_default_hook) 
    SYMBOL_ITEM_FUNC(srt6_route_search_hook) 
    SYMBOL_ITEM_FUNC(srt6_search_entry) 
    SYMBOL_ITEM_FUNC(srt6_total_entry) 
    SYMBOL_ITEM_FUNC(srt6_traversal_entry) 
    SYMBOL_ITEM_FUNC(netbd_add) 
    SYMBOL_ITEM_FUNC(netbd_add_arp) 
    SYMBOL_ITEM_FUNC(netbd_add_dev) 
    SYMBOL_ITEM_FUNC(netbd_change) 
    SYMBOL_ITEM_FUNC(netbd_delete) 
    SYMBOL_ITEM_FUNC(netbd_delete_arp) 
    SYMBOL_ITEM_FUNC(netbd_delete_dev) 
    SYMBOL_ITEM_FUNC(netbd_master_dev) 
    SYMBOL_ITEM_FUNC(netbd_show_dev) 
    SYMBOL_ITEM_FUNC(netbd_sub_delete_hook) 
    SYMBOL_ITEM_FUNC(netbr_add) 
    SYMBOL_ITEM_FUNC(netbr_add_dev) 
    SYMBOL_ITEM_FUNC(netbr_delete) 
    SYMBOL_ITEM_FUNC(netbr_delete_dev) 
    SYMBOL_ITEM_FUNC(netbr_flush_cache) 
    SYMBOL_ITEM_FUNC(netbr_show_dev) 
    SYMBOL_ITEM_FUNC(netbr_sub_delete_hook) 
    SYMBOL_ITEM_FUNC(_netEventInit) 
    SYMBOL_ITEM_FUNC(netEventIfAdd) 
    SYMBOL_ITEM_FUNC(netEventIfAddr) 
    SYMBOL_ITEM_FUNC(netEventIfAddrConflict) 
    SYMBOL_ITEM_FUNC(netEventIfAuthFail) 
    SYMBOL_ITEM_FUNC(netEventIfDown) 
    SYMBOL_ITEM_FUNC(netEventIfLink) 
    SYMBOL_ITEM_FUNC(netEventIfPppExt) 
    SYMBOL_ITEM_FUNC(netEventIfRemove) 
    SYMBOL_ITEM_FUNC(netEventIfUnlink) 
    SYMBOL_ITEM_FUNC(netEventIfUp) 
    SYMBOL_ITEM_FUNC(netEventIfWlExt) 
    SYMBOL_ITEM_FUNC(netEventIfWlExt2) 
    SYMBOL_ITEM_FUNC(netEventIfWlQual) 
    SYMBOL_ITEM_FUNC(netEventIfWlScan) 
    SYMBOL_ITEM_FUNC(fcnet_netif_attach) 
    SYMBOL_ITEM_FUNC(fcnet_netif_detach) 
    SYMBOL_ITEM_FUNC(tlsf_mem_calloc) 
    SYMBOL_ITEM_FUNC(tlsf_mem_create) 
    SYMBOL_ITEM_FUNC(tlsf_mem_free) 
    SYMBOL_ITEM_FUNC(tlsf_mem_malloc) 
    SYMBOL_ITEM_FUNC(tlsf_add_pool) 
    SYMBOL_ITEM_FUNC(tlsf_align_size) 
    SYMBOL_ITEM_FUNC(tlsf_alloc_overhead) 
    SYMBOL_ITEM_FUNC(tlsf_block_size) 
    SYMBOL_ITEM_FUNC(tlsf_block_size_max) 
    SYMBOL_ITEM_FUNC(tlsf_block_size_min) 
    SYMBOL_ITEM_FUNC(tlsf_check) 
    SYMBOL_ITEM_FUNC(tlsf_check_pool) 
    SYMBOL_ITEM_FUNC(tlsf_create) 
    SYMBOL_ITEM_FUNC(tlsf_create_with_pool) 
    SYMBOL_ITEM_FUNC(tlsf_destroy) 
    SYMBOL_ITEM_FUNC(tlsf_free) 
    SYMBOL_ITEM_FUNC(tlsf_get_pool) 
    SYMBOL_ITEM_FUNC(tlsf_malloc) 
    SYMBOL_ITEM_FUNC(tlsf_memalign) 
    SYMBOL_ITEM_FUNC(tlsf_pool_overhead) 
    SYMBOL_ITEM_FUNC(tlsf_realloc) 
    SYMBOL_ITEM_FUNC(tlsf_remove_pool) 
    SYMBOL_ITEM_FUNC(tlsf_size) 
    SYMBOL_ITEM_FUNC(tlsf_walk_pool) 
    SYMBOL_ITEM_FUNC(ip4_mrt_clean) 
    SYMBOL_ITEM_FUNC(ip4_mrt_forward) 
    SYMBOL_ITEM_FUNC(ip4_mrt_getsockopt) 
    SYMBOL_ITEM_FUNC(ip4_mrt_if_detach) 
    SYMBOL_ITEM_FUNC(ip4_mrt_ioctl) 
    SYMBOL_ITEM_FUNC(ip4_mrt_ipip_input) 
    SYMBOL_ITEM_FUNC(ip4_mrt_is_on) 
    SYMBOL_ITEM_FUNC(ip4_mrt_legal_vif_num) 
    SYMBOL_ITEM_FUNC(ip4_mrt_mcast_src) 
    SYMBOL_ITEM_FUNC(ip4_mrt_mrtstat) 
    SYMBOL_ITEM_FUNC(ip4_mrt_pim_input) 
    SYMBOL_ITEM_FUNC(ip4_mrt_pimstat) 
    SYMBOL_ITEM_FUNC(ip4_mrt_rtdetq_alloc) 
    SYMBOL_ITEM_FUNC(ip4_mrt_setsockopt) 
    SYMBOL_ITEM_FUNC(ip4_mrt_traversal_mfc) 
    SYMBOL_ITEM_FUNC(ip6_mrt_clean) 
    SYMBOL_ITEM_FUNC(ip6_mrt_forward) 
    SYMBOL_ITEM_FUNC(ip6_mrt_getsockopt) 
    SYMBOL_ITEM_FUNC(ip6_mrt_if_detach) 
    SYMBOL_ITEM_FUNC(ip6_mrt_ioctl) 
    SYMBOL_ITEM_FUNC(ip6_mrt_is_on) 
    SYMBOL_ITEM_FUNC(ip6_mrt_legal_mif_num) 
    SYMBOL_ITEM_FUNC(ip6_mrt_mrt6stat) 
    SYMBOL_ITEM_FUNC(ip6_mrt_pim_input) 
    SYMBOL_ITEM_FUNC(ip6_mrt_pim6stat) 
    SYMBOL_ITEM_FUNC(ip6_mrt_rtdetq_alloc) 
    SYMBOL_ITEM_FUNC(ip6_mrt_setsockopt) 
    SYMBOL_ITEM_FUNC(ip6_mrt_traversal_mfc) 
    SYMBOL_ITEM_FUNC(crc16) 
    SYMBOL_ITEM_FUNC(crc32_le) 
    SYMBOL_ITEM_FUNC(netdev_desc_helper_create) 
    SYMBOL_ITEM_FUNC(netdev_desc_helper_delete) 
    SYMBOL_ITEM_FUNC(netdev_desc_rx_input) 
    SYMBOL_ITEM_FUNC(netdev_desc_rx_input_offset) 
    SYMBOL_ITEM_FUNC(netdev_desc_rx_refill) 
    SYMBOL_ITEM_FUNC(netdev_desc_rx_refill_res) 
    SYMBOL_ITEM_FUNC(netdev_desc_tx_clean) 
    SYMBOL_ITEM_FUNC(netdev_desc_tx_prepare) 
    SYMBOL_ITEM_FUNC(netdev_mipif_add) 
    SYMBOL_ITEM_FUNC(netdev_mipif_clean) 
    SYMBOL_ITEM_FUNC(netdev_mipif_delete) 
    SYMBOL_ITEM_FUNC(netdev_mipif_hwaddr) 
    SYMBOL_ITEM_FUNC(netdev_mipif_search) 
    SYMBOL_ITEM_FUNC(netdev_mipif_tcpupd) 
    SYMBOL_ITEM_FUNC(netdev_mipif_update) 
    SYMBOL_ITEM_FUNC(netdev_txq_disable) 
    SYMBOL_ITEM_FUNC(netdev_txq_enable) 
    SYMBOL_ITEM_FUNC(netdev_txq_isenable) 
    SYMBOL_ITEM_FUNC(netdev_txq_length) 
    SYMBOL_ITEM_FUNC(netdev_txq_transmit) 
    SYMBOL_ITEM_FUNC(netdev_add) 
    SYMBOL_ITEM_FUNC(netdev_delete) 
    SYMBOL_ITEM_FUNC(netdev_find_by_devname) 
    SYMBOL_ITEM_FUNC(netdev_find_by_ifname) 
    SYMBOL_ITEM_FUNC(netdev_find_by_index) 
    SYMBOL_ITEM_FUNC(netdev_firewall) 
    SYMBOL_ITEM_FUNC(netdev_foreache) 
    SYMBOL_ITEM_FUNC(netdev_get_ar_hdr) 
    SYMBOL_ITEM_FUNC(netdev_get_linkup) 
    SYMBOL_ITEM_FUNC(netdev_get_tcpaf) 
    SYMBOL_ITEM_FUNC(netdev_get_tcpwnd) 
    SYMBOL_ITEM_FUNC(netdev_ifname) 
    SYMBOL_ITEM_FUNC(netdev_index) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_chkerr_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_drop_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_err_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_lenerr_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_memerr_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_recv_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkinfo_xmit_inc) 
    SYMBOL_ITEM_FUNC(netdev_linkup_wd_add) 
    SYMBOL_ITEM_FUNC(netdev_linkup_wd_delete) 
    SYMBOL_ITEM_FUNC(netdev_macfilter_add) 
    SYMBOL_ITEM_FUNC(netdev_macfilter_count) 
    SYMBOL_ITEM_FUNC(netdev_macfilter_delete) 
    SYMBOL_ITEM_FUNC(netdev_macfilter_isempty) 
    SYMBOL_ITEM_FUNC(netdev_notify) 
    SYMBOL_ITEM_FUNC(netdev_notify_clear) 
    SYMBOL_ITEM_FUNC(netdev_notify_clear_ex) 
    SYMBOL_ITEM_FUNC(netdev_notify_clear_ex_arg) 
    SYMBOL_ITEM_FUNC(netdev_notify_ex) 
    SYMBOL_ITEM_FUNC(netdev_notify_ex_arg) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_alloc) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_alloc_raw) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_can_ref) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_data) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_ethhdr) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_free) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_ip6hdr) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_iphdr) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_link) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_pull) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_push) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_tcphdr) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_trunc) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_udphdr) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_vlan_id) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_vlan_present) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_vlan_proto) 
    SYMBOL_ITEM_FUNC(netdev_pbuf_vlanhdr) 
    SYMBOL_ITEM_FUNC(netdev_poll_disable) 
    SYMBOL_ITEM_FUNC(netdev_poll_enable) 
    SYMBOL_ITEM_FUNC(netdev_poll_svc) 
    SYMBOL_ITEM_FUNC(netdev_qoshook) 
    SYMBOL_ITEM_FUNC(netdev_set_ar_hdr) 
    SYMBOL_ITEM_FUNC(netdev_set_linkup) 
    SYMBOL_ITEM_FUNC(netdev_set_tcpaf) 
    SYMBOL_ITEM_FUNC(netdev_set_tcpwnd) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_collisions_inc) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_discards_inc) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_errors_inc) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_mcasts_inc) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_total_add) 
    SYMBOL_ITEM_FUNC(netdev_statinfo_ucasts_inc) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_alloc) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_alloc_res) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_free) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_pool_create) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_pool_delete) 
    SYMBOL_ITEM_FUNC(netdev_zc_pbuf_stat) 
    SYMBOL_ITEM_FUNC(vnetdev_add) 
    SYMBOL_ITEM_FUNC(vnetdev_bufsize) 
    SYMBOL_ITEM_FUNC(vnetdev_checksum) 
    SYMBOL_ITEM_FUNC(vnetdev_delete) 
    SYMBOL_ITEM_FUNC(vnetdev_flush) 
    SYMBOL_ITEM_FUNC(vnetdev_get) 
    SYMBOL_ITEM_FUNC(vnetdev_linkup) 
    SYMBOL_ITEM_FUNC(vnetdev_maxplen) 
    SYMBOL_ITEM_FUNC(vnetdev_mtu) 
    SYMBOL_ITEM_FUNC(vnetdev_nrbytes) 
    SYMBOL_ITEM_FUNC(vnetdev_nread) 
    SYMBOL_ITEM_FUNC(vnetdev_put) 
    SYMBOL_ITEM_FUNC(__packet_have_event) 
    SYMBOL_ITEM_FUNC(__packet_set_sockfile) 
    SYMBOL_ITEM_FUNC(packet_accept) 
    SYMBOL_ITEM_FUNC(packet_bind) 
    SYMBOL_ITEM_FUNC(packet_close) 
    SYMBOL_ITEM_FUNC(packet_connect) 
    SYMBOL_ITEM_FUNC(packet_getpeername) 
    SYMBOL_ITEM_FUNC(packet_getsockname) 
    SYMBOL_ITEM_FUNC(packet_getsockopt) 
    SYMBOL_ITEM_FUNC(packet_init) 
    SYMBOL_ITEM_FUNC(packet_ioctl) 
    SYMBOL_ITEM_FUNC(packet_link_input) 
    SYMBOL_ITEM_FUNC(packet_listen) 
    SYMBOL_ITEM_FUNC(packet_mmap) 
    SYMBOL_ITEM_FUNC(packet_recv) 
    SYMBOL_ITEM_FUNC(packet_recvfrom) 
    SYMBOL_ITEM_FUNC(packet_recvmsg) 
    SYMBOL_ITEM_FUNC(packet_send) 
    SYMBOL_ITEM_FUNC(packet_sendmsg) 
    SYMBOL_ITEM_FUNC(packet_sendto) 
    SYMBOL_ITEM_FUNC(packet_setsockopt) 
    SYMBOL_ITEM_FUNC(packet_shutdown) 
    SYMBOL_ITEM_FUNC(packet_socket) 
    SYMBOL_ITEM_FUNC(packet_traversal) 
    SYMBOL_ITEM_FUNC(packet_unmap) 
    SYMBOL_ITEM_FUNC(__packetEthDgramSendto) 
    SYMBOL_ITEM_FUNC(__packetEthHeaderInfo) 
    SYMBOL_ITEM_FUNC(__packetEthHeaderInfo2) 
    SYMBOL_ITEM_FUNC(__packetEthIfInfo) 
    SYMBOL_ITEM_FUNC(__packetEthRawSendto) 
    SYMBOL_ITEM_FUNC(__procFsNetInit) 
    SYMBOL_ITEM_FUNC(__route_have_event) 
    SYMBOL_ITEM_FUNC(__route_set_sockfile) 
    SYMBOL_ITEM_FUNC(route_close) 
    SYMBOL_ITEM_FUNC(route_getsockopt) 
    SYMBOL_ITEM_FUNC(route_hook_maddr_ipv4) 
    SYMBOL_ITEM_FUNC(route_hook_maddr_ipv6) 
    SYMBOL_ITEM_FUNC(route_hook_netif_ann) 
    SYMBOL_ITEM_FUNC(route_hook_netif_ipv4) 
    SYMBOL_ITEM_FUNC(route_hook_netif_ipv6) 
    SYMBOL_ITEM_FUNC(route_hook_netif_updown) 
    SYMBOL_ITEM_FUNC(route_hook_rt_ipv4) 
    SYMBOL_ITEM_FUNC(route_hook_rt_ipv6) 
    SYMBOL_ITEM_FUNC(route_init) 
    SYMBOL_ITEM_FUNC(route_ioctl) 
    SYMBOL_ITEM_FUNC(route_recv) 
    SYMBOL_ITEM_FUNC(route_recvmsg) 
    SYMBOL_ITEM_FUNC(route_send) 
    SYMBOL_ITEM_FUNC(route_sendmsg) 
    SYMBOL_ITEM_FUNC(route_setsockopt) 
    SYMBOL_ITEM_FUNC(route_shutdown) 
    SYMBOL_ITEM_FUNC(route_socket) 
    SYMBOL_ITEM_FUNC(rt_add_entry) 
    SYMBOL_ITEM_FUNC(rt_change_default) 
    SYMBOL_ITEM_FUNC(rt_delete_default) 
    SYMBOL_ITEM_FUNC(rt_delete_entry) 
    SYMBOL_ITEM_FUNC(rt_find_entry) 
    SYMBOL_ITEM_FUNC(rt_get_default) 
    SYMBOL_ITEM_FUNC(rt_netif_add_hook) 
    SYMBOL_ITEM_FUNC(rt_netif_linkstat_hook) 
    SYMBOL_ITEM_FUNC(rt_netif_remove_hook) 
    SYMBOL_ITEM_FUNC(rt_route_gateway_hook) 
    SYMBOL_ITEM_FUNC(rt_route_search_hook) 
    SYMBOL_ITEM_FUNC(rt_total_entry) 
    SYMBOL_ITEM_FUNC(rt_traversal_entry) 
    SYMBOL_ITEM_FUNC(rt_build_sockaddr_dl) 
    SYMBOL_ITEM_FUNC(rt_entry_to_msghdr) 
    SYMBOL_ITEM_FUNC(rt_entry_to_rtentry) 
    SYMBOL_ITEM_FUNC(rt_msghdr_to_entry) 
    SYMBOL_ITEM_FUNC(rt_rtentry_to_entry) 
    SYMBOL_ITEM_FUNC(rt6_add_entry) 
    SYMBOL_ITEM_FUNC(rt6_change_default) 
    SYMBOL_ITEM_FUNC(rt6_delete_default) 
    SYMBOL_ITEM_FUNC(rt6_delete_entry) 
    SYMBOL_ITEM_FUNC(rt6_find_entry) 
    SYMBOL_ITEM_FUNC(rt6_get_default) 
    SYMBOL_ITEM_FUNC(rt6_netif_add_hook) 
    SYMBOL_ITEM_FUNC(rt6_netif_linkstat_hook) 
    SYMBOL_ITEM_FUNC(rt6_netif_remove_hook) 
    SYMBOL_ITEM_FUNC(rt6_netmask_from_prefix) 
    SYMBOL_ITEM_FUNC(rt6_netmask_to_prefix) 
    SYMBOL_ITEM_FUNC(rt6_route_gateway_hook) 
    SYMBOL_ITEM_FUNC(rt6_route_search_hook) 
    SYMBOL_ITEM_FUNC(rt6_total_entry) 
    SYMBOL_ITEM_FUNC(rt6_traversal_entry) 
    SYMBOL_ITEM_FUNC(rt6_entry_to_msghdr) 
    SYMBOL_ITEM_FUNC(rt6_entry_to_rtentry) 
    SYMBOL_ITEM_FUNC(rt6_msghdr_to_entry) 
    SYMBOL_ITEM_FUNC(rt6_rtentry_to_entry) 
    SYMBOL_ITEM_FUNC(tcp_forward_mss_adj) 
    SYMBOL_ITEM_FUNC(netconn_accept) 
    SYMBOL_ITEM_FUNC(netconn_bind) 
    SYMBOL_ITEM_FUNC(netconn_bind_if) 
    SYMBOL_ITEM_FUNC(netconn_close) 
    SYMBOL_ITEM_FUNC(netconn_connect) 
    SYMBOL_ITEM_FUNC(netconn_delete) 
    SYMBOL_ITEM_FUNC(netconn_disconnect) 
    SYMBOL_ITEM_FUNC(netconn_err) 
    SYMBOL_ITEM_FUNC(netconn_getaddr) 
    SYMBOL_ITEM_FUNC(netconn_gethostbyname_addrtype) 
    SYMBOL_ITEM_FUNC(netconn_join_leave_group) 
    SYMBOL_ITEM_FUNC(netconn_join_leave_group_netif) 
    SYMBOL_ITEM_FUNC(netconn_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(netconn_new_with_proto_and_callback) 
    SYMBOL_ITEM_FUNC(netconn_prepare_delete) 
    SYMBOL_ITEM_FUNC(netconn_recv) 
    SYMBOL_ITEM_FUNC(netconn_recv_tcp_pbuf) 
    SYMBOL_ITEM_FUNC(netconn_recv_tcp_pbuf_flags) 
    SYMBOL_ITEM_FUNC(netconn_recv_udp_raw_netbuf) 
    SYMBOL_ITEM_FUNC(netconn_recv_udp_raw_netbuf_flags) 
    SYMBOL_ITEM_FUNC(netconn_send) 
    SYMBOL_ITEM_FUNC(netconn_sendto) 
    SYMBOL_ITEM_FUNC(netconn_shutdown) 
    SYMBOL_ITEM_FUNC(netconn_tcp_recvd) 
    SYMBOL_ITEM_FUNC(netconn_thread_cleanup) 
    SYMBOL_ITEM_FUNC(netconn_thread_init) 
    SYMBOL_ITEM_FUNC(netconn_write_partly) 
    SYMBOL_ITEM_FUNC(netconn_write_vectors_partly) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_accepted) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_bind) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_bind_if) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_close) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_connect) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_delconn) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_disconnect) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_getaddr) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_gethostbyname) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_join_leave_group) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_join_leave_group_netif) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_listen) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_newconn) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_recv) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_send) 
    SYMBOL_ITEM_FUNC(lwip_netconn_do_write) 
    SYMBOL_ITEM_FUNC(lwip_netconn_is_deallocated_msg) 
    SYMBOL_ITEM_FUNC(lwip_netconn_is_err_msg) 
    SYMBOL_ITEM_FUNC(netconn_alloc) 
    SYMBOL_ITEM_FUNC(netconn_free) 
    SYMBOL_ITEM_FUNC(err_to_errno) 
    SYMBOL_ITEM_FUNC(netbuf_alloc) 
    SYMBOL_ITEM_FUNC(netbuf_chain) 
    SYMBOL_ITEM_FUNC(netbuf_data) 
    SYMBOL_ITEM_FUNC(netbuf_delete) 
    SYMBOL_ITEM_FUNC(netbuf_first) 
    SYMBOL_ITEM_FUNC(netbuf_free) 
    SYMBOL_ITEM_FUNC(netbuf_new) 
    SYMBOL_ITEM_FUNC(netbuf_next) 
    SYMBOL_ITEM_FUNC(netbuf_ref) 
    SYMBOL_ITEM_FUNC(lwip_freeaddrinfo) 
    SYMBOL_ITEM_FUNC(lwip_getaddrinfo) 
    SYMBOL_ITEM_FUNC(lwip_gethostbyname) 
    SYMBOL_ITEM_FUNC(lwip_gethostbyname_r) 
    SYMBOL_ITEM_FUNC(netifapi_arp_add) 
    SYMBOL_ITEM_FUNC(netifapi_arp_cleanup) 
    SYMBOL_ITEM_FUNC(netifapi_arp_remove) 
    SYMBOL_ITEM_FUNC(netifapi_arp_traversal) 
    SYMBOL_ITEM_FUNC(netifapi_arp_update) 
    SYMBOL_ITEM_FUNC(netifapi_nd6_cleanup) 
    SYMBOL_ITEM_FUNC(netifapi_nd6_traversal) 
    SYMBOL_ITEM_FUNC(netifapi_netif_add) 
    SYMBOL_ITEM_FUNC(netifapi_netif_common) 
    SYMBOL_ITEM_FUNC(netifapi_netif_index_to_name) 
    SYMBOL_ITEM_FUNC(netifapi_netif_name_to_index) 
    SYMBOL_ITEM_FUNC(netifapi_netif_set_addr) 
    SYMBOL_ITEM_FUNC(__lwip_have_event) 
    SYMBOL_ITEM_FUNC(__lwip_set_sockfile) 
    SYMBOL_ITEM_FUNC(lwip_accept) 
    SYMBOL_ITEM_FUNC(lwip_bind) 
    SYMBOL_ITEM_FUNC(lwip_close) 
    SYMBOL_ITEM_FUNC(lwip_connect) 
    SYMBOL_ITEM_FUNC(lwip_fcntl) 
    SYMBOL_ITEM_FUNC(lwip_getpeername) 
    SYMBOL_ITEM_FUNC(lwip_getsockname) 
    SYMBOL_ITEM_FUNC(lwip_getsockopt) 
    SYMBOL_ITEM_FUNC(lwip_inet_ntop) 
    SYMBOL_ITEM_FUNC(lwip_inet_pton) 
    SYMBOL_ITEM_FUNC(lwip_ioctl) 
    SYMBOL_ITEM_FUNC(lwip_listen) 
    SYMBOL_ITEM_FUNC(lwip_read) 
    SYMBOL_ITEM_FUNC(lwip_readv) 
    SYMBOL_ITEM_FUNC(lwip_recv) 
    SYMBOL_ITEM_FUNC(lwip_recvfrom) 
    SYMBOL_ITEM_FUNC(lwip_recvmsg) 
    SYMBOL_ITEM_FUNC(lwip_send) 
    SYMBOL_ITEM_FUNC(lwip_sendmsg) 
    SYMBOL_ITEM_FUNC(lwip_sendto) 
    SYMBOL_ITEM_FUNC(lwip_setsockopt) 
    SYMBOL_ITEM_FUNC(lwip_shutdown) 
    SYMBOL_ITEM_FUNC(lwip_socket) 
    SYMBOL_ITEM_FUNC(lwip_socket_dbg_get_socket) 
    SYMBOL_ITEM_FUNC(lwip_socket_thread_cleanup) 
    SYMBOL_ITEM_FUNC(lwip_socket_thread_init) 
    SYMBOL_ITEM_FUNC(lwip_write) 
    SYMBOL_ITEM_FUNC(lwip_writev) 
    SYMBOL_ITEM_FUNC(mem_free_callback) 
    SYMBOL_ITEM_FUNC(pbuf_free_callback) 
    SYMBOL_ITEM_FUNC(tcpip_api_call) 
    SYMBOL_ITEM_FUNC(tcpip_callback) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_delete) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_new) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_trycallback) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_trycallback_fromisr) 
    SYMBOL_ITEM_FUNC(tcpip_init) 
    SYMBOL_ITEM_FUNC(tcpip_inpkt) 
    SYMBOL_ITEM_FUNC(tcpip_inpkt_lost) 
    SYMBOL_ITEM_FUNC(tcpip_input) 
    SYMBOL_ITEM_FUNC(tcpip_qos_dontdrop_stat) 
    SYMBOL_ITEM_FUNC(tcpip_qos_get) 
    SYMBOL_ITEM_FUNC(tcpip_qos_set) 
    SYMBOL_ITEM_FUNC(tcpip_qos_stat) 
    SYMBOL_ITEM_FUNC(tcpip_send_msg_wait_sem) 
    SYMBOL_ITEM_FUNC(tcpip_try_callback) 
    SYMBOL_ITEM_FUNC(lwip_htonl) 
    SYMBOL_ITEM_FUNC(lwip_htons) 
    SYMBOL_ITEM_FUNC(lwip_itoa) 
    SYMBOL_ITEM_FUNC(lwip_stricmp) 
    SYMBOL_ITEM_FUNC(lwip_strnicmp) 
    SYMBOL_ITEM_FUNC(lwip_strnistr) 
    SYMBOL_ITEM_FUNC(lwip_strnstr) 
    SYMBOL_ITEM_FUNC(dns_gethostbyname) 
    SYMBOL_ITEM_FUNC(dns_gethostbyname_addrtype) 
    SYMBOL_ITEM_FUNC(dns_getserver) 
    SYMBOL_ITEM_FUNC(dns_init) 
    SYMBOL_ITEM_FUNC(dns_local_addhost) 
    SYMBOL_ITEM_FUNC(dns_local_iterate) 
    SYMBOL_ITEM_FUNC(dns_local_lookup) 
    SYMBOL_ITEM_FUNC(dns_local_removehost) 
    SYMBOL_ITEM_FUNC(dns_setserver) 
    SYMBOL_ITEM_FUNC(dns_tmr) 
    SYMBOL_ITEM_FUNC(inet_chksum) 
    SYMBOL_ITEM_FUNC(inet_chksum_adjust) 
    SYMBOL_ITEM_FUNC(inet_chksum_pbuf) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo_partial) 
    SYMBOL_ITEM_FUNC(ip_chksum_pseudo) 
    SYMBOL_ITEM_FUNC(ip_chksum_pseudo_partial) 
    SYMBOL_ITEM_FUNC(ip6_chksum_pseudo) 
    SYMBOL_ITEM_FUNC(ip6_chksum_pseudo_partial) 
    SYMBOL_ITEM_FUNC(lwip_standard_chksum) 
    SYMBOL_ITEM_FUNC(lwip_init) 
    SYMBOL_ITEM_FUNC(ip_input) 
    SYMBOL_ITEM_FUNC(ipaddr_aton) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa_r) 
    SYMBOL_ITEM_FUNC(mcast_block_group) 
    SYMBOL_ITEM_FUNC(mcast_block_netif) 
    SYMBOL_ITEM_FUNC(mcast_get_groupfilter_netif) 
    SYMBOL_ITEM_FUNC(mcast_get_msfilter_netif) 
    SYMBOL_ITEM_FUNC(mcast_input_local_match) 
    SYMBOL_ITEM_FUNC(mcast_ip4_filter_info) 
    SYMBOL_ITEM_FUNC(mcast_ip4_filter_interest) 
    SYMBOL_ITEM_FUNC(mcast_ip4_filter_trigger_callback) 
    SYMBOL_ITEM_FUNC(mcast_ip4_mc_find) 
    SYMBOL_ITEM_FUNC(mcast_ip6_filter_info) 
    SYMBOL_ITEM_FUNC(mcast_ip6_filter_interest) 
    SYMBOL_ITEM_FUNC(mcast_ip6_filter_trigger_callback) 
    SYMBOL_ITEM_FUNC(mcast_join_group) 
    SYMBOL_ITEM_FUNC(mcast_join_netif) 
    SYMBOL_ITEM_FUNC(mcast_leave_group) 
    SYMBOL_ITEM_FUNC(mcast_leave_netif) 
    SYMBOL_ITEM_FUNC(mcast_pcb_remove) 
    SYMBOL_ITEM_FUNC(mcast_set_groupfilter_netif) 
    SYMBOL_ITEM_FUNC(mcast_set_msfilter_netif) 
    SYMBOL_ITEM_FUNC(mcast_sock_add_drop_membership) 
    SYMBOL_ITEM_FUNC(mcast_sock_add_drop_source_membership) 
    SYMBOL_ITEM_FUNC(mcast_sock_block_unblock_source) 
    SYMBOL_ITEM_FUNC(mcast_sock_block_unblock_source_group) 
    SYMBOL_ITEM_FUNC(mcast_sock_get_groupfilter) 
    SYMBOL_ITEM_FUNC(mcast_sock_get_msfilter) 
    SYMBOL_ITEM_FUNC(mcast_sock_ipv6_add_drop_membership) 
    SYMBOL_ITEM_FUNC(mcast_sock_join_leave_group) 
    SYMBOL_ITEM_FUNC(mcast_sock_join_leave_source_group) 
    SYMBOL_ITEM_FUNC(mcast_sock_set_groupfilter) 
    SYMBOL_ITEM_FUNC(mcast_sock_set_msfilter) 
    SYMBOL_ITEM_FUNC(mcast_unblock_group) 
    SYMBOL_ITEM_FUNC(mcast_unblock_netif) 
    SYMBOL_ITEM_FUNC(mem_calloc) 
    SYMBOL_ITEM_FUNC(mem_free) 
    SYMBOL_ITEM_FUNC(mem_init) 
    SYMBOL_ITEM_FUNC(mem_malloc) 
    SYMBOL_ITEM_FUNC(mem_trim) 
    SYMBOL_ITEM_FUNC(memp_free) 
    SYMBOL_ITEM_FUNC(memp_free_pool) 
    SYMBOL_ITEM_FUNC(memp_init) 
    SYMBOL_ITEM_FUNC(memp_init_pool) 
    SYMBOL_ITEM_FUNC(memp_malloc) 
    SYMBOL_ITEM_FUNC(memp_malloc_pool) 
    SYMBOL_ITEM_FUNC(netif_add) 
    SYMBOL_ITEM_FUNC(netif_add_ext_callback) 
    SYMBOL_ITEM_FUNC(netif_add_ip6_address) 
    SYMBOL_ITEM_FUNC(netif_add_noaddr) 
    SYMBOL_ITEM_FUNC(netif_create_ip6_linklocal_address) 
    SYMBOL_ITEM_FUNC(netif_find) 
    SYMBOL_ITEM_FUNC(netif_get_by_index) 
    SYMBOL_ITEM_FUNC(netif_get_ip6_addr_match) 
    SYMBOL_ITEM_FUNC(netif_get_name) 
    SYMBOL_ITEM_FUNC(netif_get_total) 
    SYMBOL_ITEM_FUNC(netif_index_to_name) 
    SYMBOL_ITEM_FUNC(netif_init) 
    SYMBOL_ITEM_FUNC(netif_input) 
    SYMBOL_ITEM_FUNC(netif_invoke_ext_callback) 
    SYMBOL_ITEM_FUNC(netif_ip4_addr_cmp) 
    SYMBOL_ITEM_FUNC(netif_ip6_addr_set) 
    SYMBOL_ITEM_FUNC(netif_ip6_addr_set_parts) 
    SYMBOL_ITEM_FUNC(netif_ip6_addr_set_state) 
    SYMBOL_ITEM_FUNC(netif_ip6_gw_set) 
    SYMBOL_ITEM_FUNC(netif_loop_output) 
    SYMBOL_ITEM_FUNC(netif_name_to_index) 
    SYMBOL_ITEM_FUNC(netif_poll) 
    SYMBOL_ITEM_FUNC(netif_remove) 
    SYMBOL_ITEM_FUNC(netif_remove_ext_callback) 
    SYMBOL_ITEM_FUNC(netif_set_addr) 
    SYMBOL_ITEM_FUNC(netif_set_default) 
    SYMBOL_ITEM_FUNC(netif_set_down) 
    SYMBOL_ITEM_FUNC(netif_set_gw) 
    SYMBOL_ITEM_FUNC(netif_set_ipaddr) 
    SYMBOL_ITEM_FUNC(netif_set_link_callback) 
    SYMBOL_ITEM_FUNC(netif_set_link_down) 
    SYMBOL_ITEM_FUNC(netif_set_link_up) 
    SYMBOL_ITEM_FUNC(netif_set_netmask) 
    SYMBOL_ITEM_FUNC(netif_set_remove_callback) 
    SYMBOL_ITEM_FUNC(netif_set_status_callback) 
    SYMBOL_ITEM_FUNC(netif_set_up) 
    SYMBOL_ITEM_FUNC(pbuf_add_header) 
    SYMBOL_ITEM_FUNC(pbuf_add_header_force) 
    SYMBOL_ITEM_FUNC(pbuf_alloc) 
    SYMBOL_ITEM_FUNC(pbuf_alloc_reference) 
    SYMBOL_ITEM_FUNC(pbuf_alloced_custom) 
    SYMBOL_ITEM_FUNC(pbuf_cat) 
    SYMBOL_ITEM_FUNC(pbuf_chain) 
    SYMBOL_ITEM_FUNC(pbuf_clen) 
    SYMBOL_ITEM_FUNC(pbuf_clone) 
    SYMBOL_ITEM_FUNC(pbuf_coalesce) 
    SYMBOL_ITEM_FUNC(pbuf_copy) 
    SYMBOL_ITEM_FUNC(pbuf_copy_partial) 
    SYMBOL_ITEM_FUNC(pbuf_copy_partial_pbuf) 
    SYMBOL_ITEM_FUNC(pbuf_dechain) 
    SYMBOL_ITEM_FUNC(pbuf_free) 
    SYMBOL_ITEM_FUNC(pbuf_free_header) 
    SYMBOL_ITEM_FUNC(pbuf_get_at) 
    SYMBOL_ITEM_FUNC(pbuf_get_contiguous) 
    SYMBOL_ITEM_FUNC(pbuf_header) 
    SYMBOL_ITEM_FUNC(pbuf_header_force) 
    SYMBOL_ITEM_FUNC(pbuf_memcmp) 
    SYMBOL_ITEM_FUNC(pbuf_memfind) 
    SYMBOL_ITEM_FUNC(pbuf_put_at) 
    SYMBOL_ITEM_FUNC(pbuf_realloc) 
    SYMBOL_ITEM_FUNC(pbuf_ref) 
    SYMBOL_ITEM_FUNC(pbuf_remove_header) 
    SYMBOL_ITEM_FUNC(pbuf_skip) 
    SYMBOL_ITEM_FUNC(pbuf_split_64k) 
    SYMBOL_ITEM_FUNC(pbuf_strstr) 
    SYMBOL_ITEM_FUNC(pbuf_take) 
    SYMBOL_ITEM_FUNC(pbuf_take_at) 
    SYMBOL_ITEM_FUNC(pbuf_try_get_at) 
    SYMBOL_ITEM_FUNC(raw_bind) 
    SYMBOL_ITEM_FUNC(raw_bind_netif) 
    SYMBOL_ITEM_FUNC(raw_connect) 
    SYMBOL_ITEM_FUNC(raw_disconnect) 
    SYMBOL_ITEM_FUNC(raw_input) 
    SYMBOL_ITEM_FUNC(raw_netif_ip_addr_changed) 
    SYMBOL_ITEM_FUNC(raw_new) 
    SYMBOL_ITEM_FUNC(raw_new_ip_type) 
    SYMBOL_ITEM_FUNC(raw_recv) 
    SYMBOL_ITEM_FUNC(raw_remove) 
    SYMBOL_ITEM_FUNC(raw_send) 
    SYMBOL_ITEM_FUNC(raw_sendto) 
    SYMBOL_ITEM_FUNC(raw_sendto_if_src) 
    SYMBOL_ITEM_FUNC(stats_display) 
    SYMBOL_ITEM_FUNC(stats_display_igmp) 
    SYMBOL_ITEM_FUNC(stats_display_mem) 
    SYMBOL_ITEM_FUNC(stats_display_memp) 
    SYMBOL_ITEM_FUNC(stats_display_proto) 
    SYMBOL_ITEM_FUNC(stats_display_sys) 
    SYMBOL_ITEM_FUNC(stats_init) 
    SYMBOL_ITEM_FUNC(tcp_abandon) 
    SYMBOL_ITEM_FUNC(tcp_abort) 
    SYMBOL_ITEM_FUNC(tcp_accept) 
    SYMBOL_ITEM_FUNC(tcp_alloc) 
    SYMBOL_ITEM_FUNC(tcp_arg) 
    SYMBOL_ITEM_FUNC(tcp_backlog_accepted) 
    SYMBOL_ITEM_FUNC(tcp_backlog_delayed) 
    SYMBOL_ITEM_FUNC(tcp_bind) 
    SYMBOL_ITEM_FUNC(tcp_bind_netif) 
    SYMBOL_ITEM_FUNC(tcp_close) 
    SYMBOL_ITEM_FUNC(tcp_connect) 
    SYMBOL_ITEM_FUNC(tcp_debug_state_str) 
    SYMBOL_ITEM_FUNC(tcp_eff_send_mss_netif) 
    SYMBOL_ITEM_FUNC(tcp_err) 
    SYMBOL_ITEM_FUNC(tcp_fasttmr) 
    SYMBOL_ITEM_FUNC(tcp_free) 
    SYMBOL_ITEM_FUNC(tcp_free_ooseq) 
    SYMBOL_ITEM_FUNC(tcp_init) 
    SYMBOL_ITEM_FUNC(tcp_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(tcp_listen_with_backlog_and_err) 
    SYMBOL_ITEM_FUNC(tcp_netif_ip_addr_changed) 
    SYMBOL_ITEM_FUNC(tcp_new) 
    SYMBOL_ITEM_FUNC(tcp_new_ip_type) 
    SYMBOL_ITEM_FUNC(tcp_next_iss) 
    SYMBOL_ITEM_FUNC(tcp_pcb_purge) 
    SYMBOL_ITEM_FUNC(tcp_pcb_remove) 
    SYMBOL_ITEM_FUNC(tcp_poll) 
    SYMBOL_ITEM_FUNC(tcp_process_refused_data) 
    SYMBOL_ITEM_FUNC(tcp_recv) 
    SYMBOL_ITEM_FUNC(tcp_recv_null) 
    SYMBOL_ITEM_FUNC(tcp_recved) 
    SYMBOL_ITEM_FUNC(tcp_seg_copy) 
    SYMBOL_ITEM_FUNC(tcp_seg_free) 
    SYMBOL_ITEM_FUNC(tcp_segs_free) 
    SYMBOL_ITEM_FUNC(tcp_sent) 
    SYMBOL_ITEM_FUNC(tcp_setprio) 
    SYMBOL_ITEM_FUNC(tcp_shutdown) 
    SYMBOL_ITEM_FUNC(tcp_slowtmr) 
    SYMBOL_ITEM_FUNC(tcp_tcp_get_tcp_addrinfo) 
    SYMBOL_ITEM_FUNC(tcp_tmr) 
    SYMBOL_ITEM_FUNC(tcp_txnow) 
    SYMBOL_ITEM_FUNC(tcp_update_rcv_ann_wnd) 
    SYMBOL_ITEM_FUNC(tcp_input) 
    SYMBOL_ITEM_FUNC(tcp_trigger_input_pcb_close) 
    SYMBOL_ITEM_FUNC(tcp_enqueue_flags) 
    SYMBOL_ITEM_FUNC(tcp_keepalive) 
    SYMBOL_ITEM_FUNC(tcp_output) 
    SYMBOL_ITEM_FUNC(tcp_rexmit) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_fast) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_rto) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_rto_commit) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_rto_prepare) 
    SYMBOL_ITEM_FUNC(tcp_rst) 
    SYMBOL_ITEM_FUNC(tcp_send_empty_ack) 
    SYMBOL_ITEM_FUNC(tcp_send_fin) 
    SYMBOL_ITEM_FUNC(tcp_split_unsent_seg) 
    SYMBOL_ITEM_FUNC(tcp_write) 
    SYMBOL_ITEM_FUNC(tcp_zero_window_probe) 
    SYMBOL_ITEM_FUNC(sys_check_timeouts) 
    SYMBOL_ITEM_FUNC(sys_restart_timeouts) 
    SYMBOL_ITEM_FUNC(sys_timeout) 
    SYMBOL_ITEM_FUNC(sys_timeouts_init) 
    SYMBOL_ITEM_FUNC(sys_timeouts_sleeptime) 
    SYMBOL_ITEM_FUNC(sys_untimeout) 
    SYMBOL_ITEM_FUNC(tcp_timer_needed) 
    SYMBOL_ITEM_FUNC(udp_bind) 
    SYMBOL_ITEM_FUNC(udp_bind_netif) 
    SYMBOL_ITEM_FUNC(udp_connect) 
    SYMBOL_ITEM_FUNC(udp_disconnect) 
    SYMBOL_ITEM_FUNC(udp_init) 
    SYMBOL_ITEM_FUNC(udp_input) 
    SYMBOL_ITEM_FUNC(udp_netif_ip_addr_changed) 
    SYMBOL_ITEM_FUNC(udp_new) 
    SYMBOL_ITEM_FUNC(udp_new_ip_type) 
    SYMBOL_ITEM_FUNC(udp_recv) 
    SYMBOL_ITEM_FUNC(udp_remove) 
    SYMBOL_ITEM_FUNC(udp_send) 
    SYMBOL_ITEM_FUNC(udp_sendto) 
    SYMBOL_ITEM_FUNC(udp_sendto_if) 
    SYMBOL_ITEM_FUNC(udp_sendto_if_src) 
    SYMBOL_ITEM_FUNC(autoip_accept_packet) 
    SYMBOL_ITEM_FUNC(autoip_arp_reply) 
    SYMBOL_ITEM_FUNC(autoip_network_changed) 
    SYMBOL_ITEM_FUNC(autoip_set_struct) 
    SYMBOL_ITEM_FUNC(autoip_start) 
    SYMBOL_ITEM_FUNC(autoip_stop) 
    SYMBOL_ITEM_FUNC(autoip_supplied_address) 
    SYMBOL_ITEM_FUNC(autoip_tmr) 
    SYMBOL_ITEM_FUNC(dhcp_arp_reply) 
    SYMBOL_ITEM_FUNC(dhcp_cleanup) 
    SYMBOL_ITEM_FUNC(dhcp_coarse_tmr) 
    SYMBOL_ITEM_FUNC(dhcp_fine_tmr) 
    SYMBOL_ITEM_FUNC(dhcp_inform) 
    SYMBOL_ITEM_FUNC(dhcp_network_changed) 
    SYMBOL_ITEM_FUNC(dhcp_release) 
    SYMBOL_ITEM_FUNC(dhcp_release_and_stop) 
    SYMBOL_ITEM_FUNC(dhcp_renew) 
    SYMBOL_ITEM_FUNC(dhcp_set_struct) 
    SYMBOL_ITEM_FUNC(dhcp_start) 
    SYMBOL_ITEM_FUNC(dhcp_stop) 
    SYMBOL_ITEM_FUNC(dhcp_supplied_address) 
    SYMBOL_ITEM_FUNC(etharp_add_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_cleanup_netif) 
    SYMBOL_ITEM_FUNC(etharp_find_addr) 
    SYMBOL_ITEM_FUNC(etharp_get_entry) 
    SYMBOL_ITEM_FUNC(etharp_input) 
    SYMBOL_ITEM_FUNC(etharp_output) 
    SYMBOL_ITEM_FUNC(etharp_query) 
    SYMBOL_ITEM_FUNC(etharp_raw) 
    SYMBOL_ITEM_FUNC(etharp_remove_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_request) 
    SYMBOL_ITEM_FUNC(etharp_tmr) 
    SYMBOL_ITEM_FUNC(etharp_traversal) 
    SYMBOL_ITEM_FUNC(etharp_update_arp_entry) 
    SYMBOL_ITEM_FUNC(icmp_dest_unreach) 
    SYMBOL_ITEM_FUNC(icmp_input) 
    SYMBOL_ITEM_FUNC(icmp_time_exceeded) 
    SYMBOL_ITEM_FUNC(igmp_init) 
    SYMBOL_ITEM_FUNC(igmp_input) 
    SYMBOL_ITEM_FUNC(igmp_joingroup) 
    SYMBOL_ITEM_FUNC(igmp_joingroup_netif) 
    SYMBOL_ITEM_FUNC(igmp_leavegroup) 
    SYMBOL_ITEM_FUNC(igmp_leavegroup_netif) 
    SYMBOL_ITEM_FUNC(igmp_lookfor_group) 
    SYMBOL_ITEM_FUNC(igmp_report_groups) 
    SYMBOL_ITEM_FUNC(igmp_start) 
    SYMBOL_ITEM_FUNC(igmp_stop) 
    SYMBOL_ITEM_FUNC(igmp_tmr) 
    SYMBOL_ITEM_FUNC(ip4_forward_get) 
    SYMBOL_ITEM_FUNC(ip4_forward_set) 
    SYMBOL_ITEM_FUNC(ip4_get_ip_id) 
    SYMBOL_ITEM_FUNC(ip4_input) 
    SYMBOL_ITEM_FUNC(ip4_output) 
    SYMBOL_ITEM_FUNC(ip4_output_hinted) 
    SYMBOL_ITEM_FUNC(ip4_output_if) 
    SYMBOL_ITEM_FUNC(ip4_output_if_opt) 
    SYMBOL_ITEM_FUNC(ip4_output_if_opt_src) 
    SYMBOL_ITEM_FUNC(ip4_output_if_src) 
    SYMBOL_ITEM_FUNC(ip4_route) 
    SYMBOL_ITEM_FUNC(ip4_route_src) 
    SYMBOL_ITEM_FUNC(ip4_set_default_multicast_netif) 
    SYMBOL_ITEM_FUNC(ip4_addr_isbroadcast_u32) 
    SYMBOL_ITEM_FUNC(ip4_addr_netmask_valid) 
    SYMBOL_ITEM_FUNC(ip4addr_aton) 
    SYMBOL_ITEM_FUNC(ip4addr_ntoa) 
    SYMBOL_ITEM_FUNC(ip4addr_ntoa_r) 
    SYMBOL_ITEM_FUNC(ipaddr_addr) 
    SYMBOL_ITEM_FUNC(ip_reass_tmr) 
    SYMBOL_ITEM_FUNC(ip4_frag) 
    SYMBOL_ITEM_FUNC(ip4_reass) 
    SYMBOL_ITEM_FUNC(dhcp6_cleanup) 
    SYMBOL_ITEM_FUNC(dhcp6_disable) 
    SYMBOL_ITEM_FUNC(dhcp6_enable_stateful) 
    SYMBOL_ITEM_FUNC(dhcp6_enable_stateless) 
    SYMBOL_ITEM_FUNC(dhcp6_nd6_ra_trigger) 
    SYMBOL_ITEM_FUNC(dhcp6_set_struct) 
    SYMBOL_ITEM_FUNC(dhcp6_tmr) 
    SYMBOL_ITEM_FUNC(ethip6_output) 
    SYMBOL_ITEM_FUNC(icmp6_dest_unreach) 
    SYMBOL_ITEM_FUNC(icmp6_input) 
    SYMBOL_ITEM_FUNC(icmp6_packet_too_big) 
    SYMBOL_ITEM_FUNC(icmp6_param_problem) 
    SYMBOL_ITEM_FUNC(icmp6_time_exceeded) 
    SYMBOL_ITEM_FUNC(icmp6_time_exceeded_with_addrs) 
    SYMBOL_ITEM_FUNC(ip6_forward_get) 
    SYMBOL_ITEM_FUNC(ip6_forward_set) 
    SYMBOL_ITEM_FUNC(ip6_input) 
    SYMBOL_ITEM_FUNC(ip6_options_add_hbh_ra) 
    SYMBOL_ITEM_FUNC(ip6_output) 
    SYMBOL_ITEM_FUNC(ip6_output_hinted) 
    SYMBOL_ITEM_FUNC(ip6_output_if) 
    SYMBOL_ITEM_FUNC(ip6_output_if_src) 
    SYMBOL_ITEM_FUNC(ip6_route) 
    SYMBOL_ITEM_FUNC(ip6_select_source_address) 
    SYMBOL_ITEM_FUNC(ip6addr_aton) 
    SYMBOL_ITEM_FUNC(ip6addr_ntoa) 
    SYMBOL_ITEM_FUNC(ip6addr_ntoa_r) 
    SYMBOL_ITEM_FUNC(ip6_frag) 
    SYMBOL_ITEM_FUNC(ip6_reass) 
    SYMBOL_ITEM_FUNC(ip6_reass_tmr) 
    SYMBOL_ITEM_FUNC(mld6_input) 
    SYMBOL_ITEM_FUNC(mld6_joingroup) 
    SYMBOL_ITEM_FUNC(mld6_joingroup_netif) 
    SYMBOL_ITEM_FUNC(mld6_leavegroup) 
    SYMBOL_ITEM_FUNC(mld6_leavegroup_netif) 
    SYMBOL_ITEM_FUNC(mld6_lookfor_group) 
    SYMBOL_ITEM_FUNC(mld6_report_groups) 
    SYMBOL_ITEM_FUNC(mld6_stop) 
    SYMBOL_ITEM_FUNC(mld6_tmr) 
    SYMBOL_ITEM_FUNC(nd6_adjust_mld_membership) 
    SYMBOL_ITEM_FUNC(nd6_cleanup_netif) 
    SYMBOL_ITEM_FUNC(nd6_clear_destination_cache) 
    SYMBOL_ITEM_FUNC(nd6_find_route) 
    SYMBOL_ITEM_FUNC(nd6_get_destination_mtu) 
    SYMBOL_ITEM_FUNC(nd6_get_next_hop_addr_or_queue) 
    SYMBOL_ITEM_FUNC(nd6_input) 
    SYMBOL_ITEM_FUNC(nd6_reachability_hint) 
    SYMBOL_ITEM_FUNC(nd6_restart_netif) 
    SYMBOL_ITEM_FUNC(nd6_tmr) 
    SYMBOL_ITEM_FUNC(nd6_traversal) 
    SYMBOL_ITEM_FUNC(ethernet_input) 
    SYMBOL_ITEM_FUNC(ethernet_output) 
    SYMBOL_ITEM_FUNC(pktq_get) 
    SYMBOL_ITEM_FUNC(pktq_init) 
    SYMBOL_ITEM_FUNC(pktq_isempty) 
    SYMBOL_ITEM_FUNC(pktq_put) 
    SYMBOL_ITEM_FUNC(ble_addr_to_eui64) 
    SYMBOL_ITEM_FUNC(eui64_to_ble_addr) 
    SYMBOL_ITEM_FUNC(rfc7668_get_context) 
    SYMBOL_ITEM_FUNC(rfc7668_if_init) 
    SYMBOL_ITEM_FUNC(rfc7668_input) 
    SYMBOL_ITEM_FUNC(rfc7668_output) 
    SYMBOL_ITEM_FUNC(rfc7668_set_context) 
    SYMBOL_ITEM_FUNC(rfc7668_set_local_addr_eui64) 
    SYMBOL_ITEM_FUNC(rfc7668_set_local_addr_mac48) 
    SYMBOL_ITEM_FUNC(rfc7668_set_peer_addr_eui64) 
    SYMBOL_ITEM_FUNC(rfc7668_set_peer_addr_mac48) 
    SYMBOL_ITEM_FUNC(tcpip_rfc7668_input) 
    SYMBOL_ITEM_FUNC(lowpan6_compress_headers) 
    SYMBOL_ITEM_FUNC(lowpan6_decompress) 
    SYMBOL_ITEM_FUNC(lowpan6_get_address_mode) 
    SYMBOL_ITEM_FUNC(lowpan6_calc_crc) 
    SYMBOL_ITEM_FUNC(lowpan6_get_context) 
    SYMBOL_ITEM_FUNC(lowpan6_get_pan_id) 
    SYMBOL_ITEM_FUNC(lowpan6_get_short_addr) 
    SYMBOL_ITEM_FUNC(lowpan6_if_init) 
    SYMBOL_ITEM_FUNC(lowpan6_input) 
    SYMBOL_ITEM_FUNC(lowpan6_output) 
    SYMBOL_ITEM_FUNC(lowpan6_set_context) 
    SYMBOL_ITEM_FUNC(lowpan6_set_pan_id) 
    SYMBOL_ITEM_FUNC(lowpan6_set_short_addr) 
    SYMBOL_ITEM_FUNC(lowpan6_tmr) 
    SYMBOL_ITEM_FUNC(tcpip_6lowpan_input) 
    SYMBOL_ITEM_FUNC(slipif_init) 
    SYMBOL_ITEM_FUNC(slipif_poll) 
    SYMBOL_ITEM_FUNC(zepif_init) 
    SYMBOL_ITEM_FUNC(aodv_hello_process) 
    SYMBOL_ITEM_FUNC(aodv_hello_reset) 
    SYMBOL_ITEM_FUNC(aodv_hello_send) 
    SYMBOL_ITEM_FUNC(aodv_hello_start) 
    SYMBOL_ITEM_FUNC(aodv_hello_stop) 
    SYMBOL_ITEM_FUNC(aodv_hello_update_timeout) 
    SYMBOL_ITEM_FUNC(aodv_expanding_ring_search_get) 
    SYMBOL_ITEM_FUNC(aodv_expanding_ring_search_set) 
    SYMBOL_ITEM_FUNC(aodv_gw_get) 
    SYMBOL_ITEM_FUNC(aodv_gw_set) 
    SYMBOL_ITEM_FUNC(aodv_netif_add) 
    SYMBOL_ITEM_FUNC(aodv_netif_output) 
    SYMBOL_ITEM_FUNC(aodv_netif_output6) 
    SYMBOL_ITEM_FUNC(aodv_netif_remove) 
    SYMBOL_ITEM_FUNC(aodv_wait_on_reboot_get) 
    SYMBOL_ITEM_FUNC(aodv_neighbor_add) 
    SYMBOL_ITEM_FUNC(aodv_neighbor_link_break) 
    SYMBOL_ITEM_FUNC(aodv_udp_new) 
    SYMBOL_ITEM_FUNC(aodv_udp_remove) 
    SYMBOL_ITEM_FUNC(aodv_udp_sendto) 
    SYMBOL_ITEM_FUNC(aodv_rerr_add_udest) 
    SYMBOL_ITEM_FUNC(aodv_rerr_create) 
    SYMBOL_ITEM_FUNC(aodv_rerr_process) 
    SYMBOL_ITEM_FUNC(aodv_precursor_add) 
    SYMBOL_ITEM_FUNC(aodv_precursor_list_destroy) 
    SYMBOL_ITEM_FUNC(aodv_precursor_remove) 
    SYMBOL_ITEM_FUNC(aodv_rt_delete) 
    SYMBOL_ITEM_FUNC(aodv_rt_delete_netif) 
    SYMBOL_ITEM_FUNC(aodv_rt_find) 
    SYMBOL_ITEM_FUNC(aodv_rt_find_gateway) 
    SYMBOL_ITEM_FUNC(aodv_rt_init) 
    SYMBOL_ITEM_FUNC(aodv_rt_insert) 
    SYMBOL_ITEM_FUNC(aodv_rt_invalidate) 
    SYMBOL_ITEM_FUNC(aodv_rt_netif_index_get) 
    SYMBOL_ITEM_FUNC(aodv_rt_traversal) 
    SYMBOL_ITEM_FUNC(aodv_rt_update) 
    SYMBOL_ITEM_FUNC(aodv_rt_update_inet_rt) 
    SYMBOL_ITEM_FUNC(aodv_rt_update_route_timeouts) 
    SYMBOL_ITEM_FUNC(aodv_rt_update_timeout) 
    SYMBOL_ITEM_FUNC(aodv_rrep_ack_create) 
    SYMBOL_ITEM_FUNC(aodv_rrep_ack_process) 
    SYMBOL_ITEM_FUNC(aodv_rrep_create) 
    SYMBOL_ITEM_FUNC(aodv_rrep_ext_add) 
    SYMBOL_ITEM_FUNC(aodv_rrep_forward) 
    SYMBOL_ITEM_FUNC(aodv_rrep_process) 
    SYMBOL_ITEM_FUNC(aodv_rrep_send) 
    SYMBOL_ITEM_FUNC(aodv_rreq_blacklist_find) 
    SYMBOL_ITEM_FUNC(aodv_rreq_blacklist_insert) 
    SYMBOL_ITEM_FUNC(aodv_rreq_blacklist_timeout) 
    SYMBOL_ITEM_FUNC(aodv_rreq_create) 
    SYMBOL_ITEM_FUNC(aodv_rreq_forward) 
    SYMBOL_ITEM_FUNC(aodv_rreq_local_repair) 
    SYMBOL_ITEM_FUNC(aodv_rreq_process) 
    SYMBOL_ITEM_FUNC(aodv_rreq_record_find) 
    SYMBOL_ITEM_FUNC(aodv_rreq_record_insert) 
    SYMBOL_ITEM_FUNC(aodv_rreq_record_timeout) 
    SYMBOL_ITEM_FUNC(aodv_rreq_route_discovery) 
    SYMBOL_ITEM_FUNC(aodv_rreq_send) 
    SYMBOL_ITEM_FUNC(aodv_rreq_send_ext) 
    SYMBOL_ITEM_FUNC(aodv_seeklist_find) 
    SYMBOL_ITEM_FUNC(aodv_seeklist_insert) 
    SYMBOL_ITEM_FUNC(aodv_seeklist_remove) 
    SYMBOL_ITEM_FUNC(aodv_timer_add) 
    SYMBOL_ITEM_FUNC(aodv_timer_init) 
    SYMBOL_ITEM_FUNC(aodv_timer_init_svr) 
    SYMBOL_ITEM_FUNC(aodv_timer_left) 
    SYMBOL_ITEM_FUNC(aodv_timer_remove) 
    SYMBOL_ITEM_FUNC(aodv_timer_set_timeout) 
    SYMBOL_ITEM_FUNC(aodv_timer_timeout_now) 
    SYMBOL_ITEM_FUNC(aodv_hello_timeout) 
    SYMBOL_ITEM_FUNC(aodv_local_repair_timeout) 
    SYMBOL_ITEM_FUNC(aodv_route_delete_timeout) 
    SYMBOL_ITEM_FUNC(aodv_route_discovery_timeout) 
    SYMBOL_ITEM_FUNC(aodv_route_expire_timeout) 
    SYMBOL_ITEM_FUNC(aodv_rrep_ack_timeout) 
    SYMBOL_ITEM_FUNC(aodv_wait_on_reboot_timeout) 
    SYMBOL_ITEM_FUNC(auth_withpeer_fail) 
    SYMBOL_ITEM_FUNC(auth_withpeer_success) 
    SYMBOL_ITEM_FUNC(continue_networks) 
    SYMBOL_ITEM_FUNC(get_secret) 
    SYMBOL_ITEM_FUNC(link_down) 
    SYMBOL_ITEM_FUNC(link_established) 
    SYMBOL_ITEM_FUNC(link_required) 
    SYMBOL_ITEM_FUNC(link_terminated) 
    SYMBOL_ITEM_FUNC(np_down) 
    SYMBOL_ITEM_FUNC(np_finished) 
    SYMBOL_ITEM_FUNC(np_up) 
    SYMBOL_ITEM_FUNC(start_networks) 
    SYMBOL_ITEM_FUNC(upper_layers_down) 
    SYMBOL_ITEM_FUNC(chap_auth_with_peer) 
    SYMBOL_ITEM_FUNC(eap_authwithpeer) 
    SYMBOL_ITEM_FUNC(eui64_ntoa) 
    SYMBOL_ITEM_FUNC(fsm_close) 
    SYMBOL_ITEM_FUNC(fsm_init) 
    SYMBOL_ITEM_FUNC(fsm_input) 
    SYMBOL_ITEM_FUNC(fsm_lowerdown) 
    SYMBOL_ITEM_FUNC(fsm_lowerup) 
    SYMBOL_ITEM_FUNC(fsm_open) 
    SYMBOL_ITEM_FUNC(fsm_protreject) 
    SYMBOL_ITEM_FUNC(fsm_sdata) 
    SYMBOL_ITEM_FUNC(lcp_close) 
    SYMBOL_ITEM_FUNC(lcp_lowerdown) 
    SYMBOL_ITEM_FUNC(lcp_lowerup) 
    SYMBOL_ITEM_FUNC(lcp_open) 
    SYMBOL_ITEM_FUNC(lcp_sprotrej) 
    SYMBOL_ITEM_FUNC(magic) 
    SYMBOL_ITEM_FUNC(magic_init) 
    SYMBOL_ITEM_FUNC(magic_pow) 
    SYMBOL_ITEM_FUNC(magic_random_bytes) 
    SYMBOL_ITEM_FUNC(magic_randomize) 
    SYMBOL_ITEM_FUNC(cdns) 
    SYMBOL_ITEM_FUNC(cif6addr) 
    SYMBOL_ITEM_FUNC(cifaddr) 
    SYMBOL_ITEM_FUNC(get_mask) 
    SYMBOL_ITEM_FUNC(netif_get_mtu) 
    SYMBOL_ITEM_FUNC(netif_set_mtu) 
    SYMBOL_ITEM_FUNC(new_phase) 
    SYMBOL_ITEM_FUNC(ppp_close) 
    SYMBOL_ITEM_FUNC(ppp_connect) 
    SYMBOL_ITEM_FUNC(ppp_free) 
    SYMBOL_ITEM_FUNC(ppp_init) 
    SYMBOL_ITEM_FUNC(ppp_input) 
    SYMBOL_ITEM_FUNC(ppp_ioctl) 
    SYMBOL_ITEM_FUNC(ppp_link_end) 
    SYMBOL_ITEM_FUNC(ppp_link_failed) 
    SYMBOL_ITEM_FUNC(ppp_link_terminated) 
    SYMBOL_ITEM_FUNC(ppp_new) 
    SYMBOL_ITEM_FUNC(ppp_recv_config) 
    SYMBOL_ITEM_FUNC(ppp_send_config) 
    SYMBOL_ITEM_FUNC(ppp_set_auth) 
    SYMBOL_ITEM_FUNC(ppp_set_notify_phase_callback) 
    SYMBOL_ITEM_FUNC(ppp_start) 
    SYMBOL_ITEM_FUNC(ppp_write) 
    SYMBOL_ITEM_FUNC(sdns) 
    SYMBOL_ITEM_FUNC(sif6addr) 
    SYMBOL_ITEM_FUNC(sif6down) 
    SYMBOL_ITEM_FUNC(sif6up) 
    SYMBOL_ITEM_FUNC(sifaddr) 
    SYMBOL_ITEM_FUNC(sifdown) 
    SYMBOL_ITEM_FUNC(sifup) 
    SYMBOL_ITEM_FUNC(sifvjcomp) 
    SYMBOL_ITEM_FUNC(pppapi_close) 
    SYMBOL_ITEM_FUNC(pppapi_connect) 
    SYMBOL_ITEM_FUNC(pppapi_free) 
    SYMBOL_ITEM_FUNC(pppapi_ioctl) 
    SYMBOL_ITEM_FUNC(pppapi_pppoe_create) 
    SYMBOL_ITEM_FUNC(pppapi_pppol2tp_create) 
    SYMBOL_ITEM_FUNC(pppapi_pppos_create) 
    SYMBOL_ITEM_FUNC(pppapi_set_default) 
    SYMBOL_ITEM_FUNC(pppapi_set_notify_phase_callback) 
    SYMBOL_ITEM_FUNC(pppcrypt_56_to_64_bit_key) 
    SYMBOL_ITEM_FUNC(pppoe_create) 
    SYMBOL_ITEM_FUNC(pppoe_data_input) 
    SYMBOL_ITEM_FUNC(pppoe_disc_input) 
    SYMBOL_ITEM_FUNC(pppol2tp_create) 
    SYMBOL_ITEM_FUNC(pppos_create) 
    SYMBOL_ITEM_FUNC(pppos_input) 
    SYMBOL_ITEM_FUNC(upap_authwithpeer) 
    SYMBOL_ITEM_FUNC(ppp_dbglog_impl) 
    SYMBOL_ITEM_FUNC(ppp_error_impl) 
    SYMBOL_ITEM_FUNC(ppp_fatal_impl) 
    SYMBOL_ITEM_FUNC(ppp_info_impl) 
    SYMBOL_ITEM_FUNC(ppp_notice_impl) 
    SYMBOL_ITEM_FUNC(ppp_print_string) 
    SYMBOL_ITEM_FUNC(ppp_slprintf) 
    SYMBOL_ITEM_FUNC(ppp_strlcat) 
    SYMBOL_ITEM_FUNC(ppp_strlcpy) 
    SYMBOL_ITEM_FUNC(ppp_vslprintf) 
    SYMBOL_ITEM_FUNC(ppp_warn_impl) 
    SYMBOL_ITEM_FUNC(vj_compress_init) 
    SYMBOL_ITEM_FUNC(vj_compress_tcp) 
    SYMBOL_ITEM_FUNC(vj_uncompress_err) 
    SYMBOL_ITEM_FUNC(vj_uncompress_tcp) 
    SYMBOL_ITEM_FUNC(vj_uncompress_uncomp) 
    SYMBOL_ITEM_FUNC(crypt_decode) 
    SYMBOL_ITEM_FUNC(crypt_encode) 
    SYMBOL_ITEM_FUNC(crypt_init) 
    SYMBOL_ITEM_FUNC(crypt_verify) 
    SYMBOL_ITEM_FUNC(ieee802154_from_eth) 
    SYMBOL_ITEM_FUNC(ieee802154_hdr_len) 
    SYMBOL_ITEM_FUNC(ieee802154_seqno_set) 
    SYMBOL_ITEM_FUNC(ieee802154_to_eth) 
    SYMBOL_ITEM_FUNC(ieee802154_frame_create_ack) 
    SYMBOL_ITEM_FUNC(ieee802154_frame_create_hdr) 
    SYMBOL_ITEM_FUNC(ieee802154_frame_hdrlen) 
    SYMBOL_ITEM_FUNC(ieee802154_frame_parse) 
    SYMBOL_ITEM_FUNC(lowpan_context_addr_add) 
    SYMBOL_ITEM_FUNC(lowpan_context_addr_delete) 
    SYMBOL_ITEM_FUNC(lowpan_header_create) 
    SYMBOL_ITEM_FUNC(lowpan_header_prase) 
    SYMBOL_ITEM_FUNC(lowpan_frag) 
    SYMBOL_ITEM_FUNC(lowpan_reass) 
    SYMBOL_ITEM_FUNC(lowpan_reass_init) 
    SYMBOL_ITEM_FUNC(lowpan_reass_tmr) 
    SYMBOL_ITEM_FUNC(lowpan_input) 
    SYMBOL_ITEM_FUNC(lowpan_netif_add) 
    SYMBOL_ITEM_FUNC(lowpan_netif_autoip_start) 
    SYMBOL_ITEM_FUNC(lowpan_netif_dhcp_start) 
    SYMBOL_ITEM_FUNC(lowpan_netif_set_default) 
    SYMBOL_ITEM_FUNC(lowpan_netif_set_up) 
    SYMBOL_ITEM_FUNC(mac_input_callback) 
    SYMBOL_ITEM_FUNC(mac_send_callback) 
    SYMBOL_ITEM_FUNC(API_INetFtpServerBindDev) 
    SYMBOL_ITEM_FUNC(API_INetFtpServerInit) 
    SYMBOL_ITEM_FUNC(API_INetFtpServerPath) 
    SYMBOL_ITEM_FUNC(API_INetFtpServerShow) 
    SYMBOL_ITEM_FUNC(__inetHostTableGetItem) 
    SYMBOL_ITEM_FUNC(__inetHostTableInit) 
    SYMBOL_ITEM_FUNC(API_INetHostTableAddItem) 
    SYMBOL_ITEM_FUNC(API_INetHostTableDelItem) 
    SYMBOL_ITEM_FUNC(API_INetHostTableGetItem) 
    SYMBOL_ITEM_FUNC(__inetIacFilter) 
    SYMBOL_ITEM_FUNC(__inetIacMakeFrame) 
    SYMBOL_ITEM_FUNC(__inetIacSend) 
    SYMBOL_ITEM_FUNC(API_INetNatAliasAdd) 
    SYMBOL_ITEM_FUNC(API_INetNatAliasDelete) 
    SYMBOL_ITEM_FUNC(API_INetNatInit) 
    SYMBOL_ITEM_FUNC(API_INetNatIpFragGet) 
    SYMBOL_ITEM_FUNC(API_INetNatIpFragSet) 
    SYMBOL_ITEM_FUNC(API_INetNatLocalAdd) 
    SYMBOL_ITEM_FUNC(API_INetNatLocalDelete) 
    SYMBOL_ITEM_FUNC(API_INetNatMapAdd) 
    SYMBOL_ITEM_FUNC(API_INetNatMapDelete) 
    SYMBOL_ITEM_FUNC(API_INetNatStart) 
    SYMBOL_ITEM_FUNC(API_INetNatStop) 
    SYMBOL_ITEM_FUNC(API_INetNatWanAdd) 
    SYMBOL_ITEM_FUNC(API_INetNatWanDelete) 
    SYMBOL_ITEM_FUNC(__natAddAp) 
    SYMBOL_ITEM_FUNC(__natAddLocal) 
    SYMBOL_ITEM_FUNC(__natAliasAdd) 
    SYMBOL_ITEM_FUNC(__natAliasDelete) 
    SYMBOL_ITEM_FUNC(__natDeleteAp) 
    SYMBOL_ITEM_FUNC(__natDeleteLocal) 
    SYMBOL_ITEM_FUNC(__natInit) 
    SYMBOL_ITEM_FUNC(__natIpFragGet) 
    SYMBOL_ITEM_FUNC(__natIpFragSet) 
    SYMBOL_ITEM_FUNC(__natMapAdd) 
    SYMBOL_ITEM_FUNC(__natMapDelete) 
    SYMBOL_ITEM_FUNC(__natStart) 
    SYMBOL_ITEM_FUNC(__natStop) 
    SYMBOL_ITEM_FUNC(__procFsNatInit) 
    SYMBOL_ITEM_FUNC(nat_netif_add_hook) 
    SYMBOL_ITEM_FUNC(nat_netif_remove_hook) 
    SYMBOL_ITEM_FUNC(API_INetNetBiosBindDev) 
    SYMBOL_ITEM_FUNC(API_INetNetBiosInit) 
    SYMBOL_ITEM_FUNC(API_INetNetBiosNameGet) 
    SYMBOL_ITEM_FUNC(API_INetNetBiosNameSet) 
    SYMBOL_ITEM_FUNC(netbiosns_binddev) 
    SYMBOL_ITEM_FUNC(netbiosns_init) 
    SYMBOL_ITEM_FUNC(netbiosns_stop) 
    SYMBOL_ITEM_FUNC(API_INetNpfAllowGet) 
    SYMBOL_ITEM_FUNC(API_INetNpfDropGet) 
    SYMBOL_ITEM_FUNC(API_INetNpfInit) 
    SYMBOL_ITEM_FUNC(API_INetNpfRuleAdd) 
    SYMBOL_ITEM_FUNC(API_INetNpfRuleAddEx) 
    SYMBOL_ITEM_FUNC(API_INetNpfRuleDel) 
    SYMBOL_ITEM_FUNC(API_INetNpfRuleDelEx) 
    SYMBOL_ITEM_FUNC(API_INetNpfShow) 
    SYMBOL_ITEM_FUNC(npf_netif_attach) 
    SYMBOL_ITEM_FUNC(npf_netif_detach) 
    SYMBOL_ITEM_FUNC(API_INetPing) 
    SYMBOL_ITEM_FUNC(API_INetPingInit) 
    SYMBOL_ITEM_FUNC(API_INetPing6) 
    SYMBOL_ITEM_FUNC(API_INetPing6Init) 
    SYMBOL_ITEM_FUNC(API_PppConnect) 
    SYMBOL_ITEM_FUNC(API_PppDelete) 
    SYMBOL_ITEM_FUNC(API_PppDisconnect) 
    SYMBOL_ITEM_FUNC(API_PppGetPhase) 
    SYMBOL_ITEM_FUNC(API_PppOeCreate) 
    SYMBOL_ITEM_FUNC(API_PppOl2tpCreate) 
    SYMBOL_ITEM_FUNC(API_PppOsCreate) 
    SYMBOL_ITEM_FUNC(API_INetQosInit) 
    SYMBOL_ITEM_FUNC(API_INetQosRuleAdd) 
    SYMBOL_ITEM_FUNC(API_INetQosRuleDel) 
    SYMBOL_ITEM_FUNC(API_INetQosShow) 
    SYMBOL_ITEM_FUNC(qos_netif_attach) 
    SYMBOL_ITEM_FUNC(qos_netif_detach) 
    SYMBOL_ITEM_FUNC(xdr_authdes_cred) 
    SYMBOL_ITEM_FUNC(xdr_authdes_verf) 
    SYMBOL_ITEM_FUNC(xdr_authunix_parms) 
    SYMBOL_ITEM_FUNC(authnone_create) 
    SYMBOL_ITEM_FUNC(authunix_create) 
    SYMBOL_ITEM_FUNC(authunix_create_default) 
    SYMBOL_ITEM_FUNC(bindresvport) 
    SYMBOL_ITEM_FUNC(clnt_create) 
    SYMBOL_ITEM_FUNC(clnt_pcreateerror) 
    SYMBOL_ITEM_FUNC(clnt_perrno) 
    SYMBOL_ITEM_FUNC(clnt_perror) 
    SYMBOL_ITEM_FUNC(clnt_spcreateerror) 
    SYMBOL_ITEM_FUNC(clnt_sperrno) 
    SYMBOL_ITEM_FUNC(clnt_sperror) 
    SYMBOL_ITEM_FUNC(libc_freeres_fn) 
    SYMBOL_ITEM_FUNC(clntraw_create) 
    SYMBOL_ITEM_FUNC(callrpc) 
    SYMBOL_ITEM_FUNC(clnttcp_create) 
    SYMBOL_ITEM_FUNC(__libc_clntudp_bufcreate) 
    SYMBOL_ITEM_FUNC(clntudp_bufcreate) 
    SYMBOL_ITEM_FUNC(clntudp_create) 
    SYMBOL_ITEM_FUNC(clntunix_create) 
    SYMBOL_ITEM_FUNC(_create_xid) 
    SYMBOL_ITEM_FUNC(cbc_crypt) 
    SYMBOL_ITEM_FUNC(ecb_crypt) 
    SYMBOL_ITEM_FUNC(_des_crypt) 
    SYMBOL_ITEM_FUNC(des_setparity) 
    SYMBOL_ITEM_FUNC(__getprotobyname_r) 
    SYMBOL_ITEM_FUNC(getrpcport) 
    SYMBOL_ITEM_FUNC(get_myaddress) 
    SYMBOL_ITEM_FUNC(key_decryptsession) 
    SYMBOL_ITEM_FUNC(key_decryptsession_pk) 
    SYMBOL_ITEM_FUNC(key_encryptsession) 
    SYMBOL_ITEM_FUNC(key_encryptsession_pk) 
    SYMBOL_ITEM_FUNC(key_gendes) 
    SYMBOL_ITEM_FUNC(key_get_conv) 
    SYMBOL_ITEM_FUNC(key_secretkey_is_set) 
    SYMBOL_ITEM_FUNC(key_setnet) 
    SYMBOL_ITEM_FUNC(key_setsecret) 
    SYMBOL_ITEM_FUNC(xdr_cryptkeyarg) 
    SYMBOL_ITEM_FUNC(xdr_cryptkeyarg2) 
    SYMBOL_ITEM_FUNC(xdr_cryptkeyres) 
    SYMBOL_ITEM_FUNC(xdr_getcredres) 
    SYMBOL_ITEM_FUNC(xdr_key_netstarg) 
    SYMBOL_ITEM_FUNC(xdr_key_netstres) 
    SYMBOL_ITEM_FUNC(xdr_keybuf) 
    SYMBOL_ITEM_FUNC(xdr_keystatus) 
    SYMBOL_ITEM_FUNC(xdr_netnamestr) 
    SYMBOL_ITEM_FUNC(xdr_unixcred) 
    SYMBOL_ITEM_FUNC(pmap_set) 
    SYMBOL_ITEM_FUNC(pmap_unset) 
    SYMBOL_ITEM_FUNC(xdr_pmap) 
    SYMBOL_ITEM_FUNC(xdr_pmaplist) 
    SYMBOL_ITEM_FUNC(clnt_broadcast) 
    SYMBOL_ITEM_FUNC(pmap_rmtcall) 
    SYMBOL_ITEM_FUNC(xdr_rmtcall_args) 
    SYMBOL_ITEM_FUNC(xdr_rmtcallres) 
    SYMBOL_ITEM_FUNC(pmap_getmaps) 
    SYMBOL_ITEM_FUNC(__get_socket) 
    SYMBOL_ITEM_FUNC(__libc_rpc_getport) 
    SYMBOL_ITEM_FUNC(pmap_getport) 
    SYMBOL_ITEM_FUNC(xdr_callmsg) 
    SYMBOL_ITEM_FUNC(_rpc_dtablesize) 
    SYMBOL_ITEM_FUNC(_seterr_reply) 
    SYMBOL_ITEM_FUNC(xdr_accepted_reply) 
    SYMBOL_ITEM_FUNC(xdr_callhdr) 
    SYMBOL_ITEM_FUNC(xdr_des_block) 
    SYMBOL_ITEM_FUNC(xdr_opaque_auth) 
    SYMBOL_ITEM_FUNC(xdr_rejected_reply) 
    SYMBOL_ITEM_FUNC(xdr_replymsg) 
    SYMBOL_ITEM_FUNC(rtime) 
    SYMBOL_ITEM_FUNC(__svc_accept_failed) 
    SYMBOL_ITEM_FUNC(svc_getreq) 
    SYMBOL_ITEM_FUNC(svc_getreq_common) 
    SYMBOL_ITEM_FUNC(svc_getreq_poll) 
    SYMBOL_ITEM_FUNC(svc_getreqset) 
    SYMBOL_ITEM_FUNC(svc_register) 
    SYMBOL_ITEM_FUNC(svc_sendreply) 
    SYMBOL_ITEM_FUNC(svc_unregister) 
    SYMBOL_ITEM_FUNC(svcerr_auth) 
    SYMBOL_ITEM_FUNC(svcerr_decode) 
    SYMBOL_ITEM_FUNC(svcerr_noproc) 
    SYMBOL_ITEM_FUNC(svcerr_noprog) 
    SYMBOL_ITEM_FUNC(svcerr_progvers) 
    SYMBOL_ITEM_FUNC(svcerr_systemerr) 
    SYMBOL_ITEM_FUNC(svcerr_weakauth) 
    SYMBOL_ITEM_FUNC(xprt_register) 
    SYMBOL_ITEM_FUNC(xprt_unregister) 
    SYMBOL_ITEM_FUNC(_authenticate) 
    SYMBOL_ITEM_FUNC(_svcauth_short) 
    SYMBOL_ITEM_FUNC(_svcauth_unix) 
    SYMBOL_ITEM_FUNC(svcraw_create) 
    SYMBOL_ITEM_FUNC(svc_exit) 
    SYMBOL_ITEM_FUNC(svc_run) 
    SYMBOL_ITEM_FUNC(__registerrpc) 
    SYMBOL_ITEM_FUNC(svcfd_create) 
    SYMBOL_ITEM_FUNC(svctcp_create) 
    SYMBOL_ITEM_FUNC(svcudp_bufcreate) 
    SYMBOL_ITEM_FUNC(svcudp_create) 
    SYMBOL_ITEM_FUNC(svcudp_enablecache) 
    SYMBOL_ITEM_FUNC(svcunix_create) 
    SYMBOL_ITEM_FUNC(svcunixfd_create) 
    SYMBOL_ITEM_FUNC(passwd2des) 
    SYMBOL_ITEM_FUNC(passwd2des_internal) 
    SYMBOL_ITEM_FUNC(xdecrypt) 
    SYMBOL_ITEM_FUNC(xencrypt) 
    SYMBOL_ITEM_FUNC(xdr_bool) 
    SYMBOL_ITEM_FUNC(xdr_bytes) 
    SYMBOL_ITEM_FUNC(xdr_char) 
    SYMBOL_ITEM_FUNC(xdr_enum) 
    SYMBOL_ITEM_FUNC(xdr_free) 
    SYMBOL_ITEM_FUNC(xdr_hyper) 
    SYMBOL_ITEM_FUNC(xdr_int) 
    SYMBOL_ITEM_FUNC(xdr_long) 
    SYMBOL_ITEM_FUNC(xdr_longlong_t) 
    SYMBOL_ITEM_FUNC(xdr_netobj) 
    SYMBOL_ITEM_FUNC(xdr_opaque) 
    SYMBOL_ITEM_FUNC(xdr_short) 
    SYMBOL_ITEM_FUNC(xdr_string) 
    SYMBOL_ITEM_FUNC(xdr_u_char) 
    SYMBOL_ITEM_FUNC(xdr_u_hyper) 
    SYMBOL_ITEM_FUNC(xdr_u_int) 
    SYMBOL_ITEM_FUNC(xdr_u_long) 
    SYMBOL_ITEM_FUNC(xdr_u_longlong_t) 
    SYMBOL_ITEM_FUNC(xdr_u_short) 
    SYMBOL_ITEM_FUNC(xdr_union) 
    SYMBOL_ITEM_FUNC(xdr_void) 
    SYMBOL_ITEM_FUNC(xdr_wrapstring) 
    SYMBOL_ITEM_FUNC(xdr_array) 
    SYMBOL_ITEM_FUNC(xdr_vector) 
    SYMBOL_ITEM_FUNC(xdr_double) 
    SYMBOL_ITEM_FUNC(xdr_float) 
    SYMBOL_ITEM_FUNC(xdr_int16_t) 
    SYMBOL_ITEM_FUNC(xdr_int32_t) 
    SYMBOL_ITEM_FUNC(xdr_int64_t) 
    SYMBOL_ITEM_FUNC(xdr_int8_t) 
    SYMBOL_ITEM_FUNC(xdr_quad_t) 
    SYMBOL_ITEM_FUNC(xdr_u_quad_t) 
    SYMBOL_ITEM_FUNC(xdr_uint16_t) 
    SYMBOL_ITEM_FUNC(xdr_uint32_t) 
    SYMBOL_ITEM_FUNC(xdr_uint64_t) 
    SYMBOL_ITEM_FUNC(xdr_uint8_t) 
    SYMBOL_ITEM_FUNC(xdrmem_create) 
    SYMBOL_ITEM_FUNC(xdrrec_create) 
    SYMBOL_ITEM_FUNC(xdrrec_endofrecord) 
    SYMBOL_ITEM_FUNC(xdrrec_eof) 
    SYMBOL_ITEM_FUNC(xdrrec_skiprecord) 
    SYMBOL_ITEM_FUNC(xdr_pointer) 
    SYMBOL_ITEM_FUNC(xdr_reference) 
    SYMBOL_ITEM_FUNC(xdr_sizeof) 
    SYMBOL_ITEM_FUNC(xdrstdio_create) 
    SYMBOL_ITEM_FUNC(snmpv3_build_priv_param) 
    SYMBOL_ITEM_FUNC(snmpv3_engine_id_changed) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_boots_internal) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_time_internal) 
    SYMBOL_ITEM_FUNC(snmpv3_auth) 
    SYMBOL_ITEM_FUNC(snmpv3_crypt) 
    SYMBOL_ITEM_FUNC(snmpv3_password_to_key_md5) 
    SYMBOL_ITEM_FUNC(snmpv3_password_to_key_sha) 
    SYMBOL_ITEM_FUNC(parse_user_config) 
    SYMBOL_ITEM_FUNC(snmpv3_get_amount_of_users) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_boots) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_boots_config) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_id) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_id_config) 
    SYMBOL_ITEM_FUNC(snmpv3_get_engine_time) 
    SYMBOL_ITEM_FUNC(snmpv3_get_user) 
    SYMBOL_ITEM_FUNC(snmpv3_get_user_storagetype) 
    SYMBOL_ITEM_FUNC(snmpv3_get_user_table_config) 
    SYMBOL_ITEM_FUNC(snmpv3_get_username) 
    SYMBOL_ITEM_FUNC(snmpv3_reset_engine_boots) 
    SYMBOL_ITEM_FUNC(snmpv3_reset_engine_id_config) 
    SYMBOL_ITEM_FUNC(snmpv3_reset_engine_time) 
    SYMBOL_ITEM_FUNC(snmpv3_reset_user_table) 
    SYMBOL_ITEM_FUNC(snmpv3_save_engine_boots_config) 
    SYMBOL_ITEM_FUNC(snmpv3_save_engine_id_config) 
    SYMBOL_ITEM_FUNC(snmpv3_set_engine_boots) 
    SYMBOL_ITEM_FUNC(snmpv3_set_engine_id) 
    SYMBOL_ITEM_FUNC(snmpv3_set_user_auth_algo) 
    SYMBOL_ITEM_FUNC(snmpv3_set_user_auth_key) 
    SYMBOL_ITEM_FUNC(snmpv3_set_user_priv_algo) 
    SYMBOL_ITEM_FUNC(snmpv3_set_user_priv_key) 
    SYMBOL_ITEM_FUNC(snmpv3_sylixos_init) 
    SYMBOL_ITEM_FUNC(snmp_ans1_enc_tlv) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_oid) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_raw) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_s32t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_tlv) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_u32t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_dec_u64t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_length_cnt) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_oid) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_oid_cnt) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_raw) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_s32t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_s32t_cnt) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_u32t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_u32t_cnt) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_u64t) 
    SYMBOL_ITEM_FUNC(snmp_asn1_enc_u64t_cnt) 
    SYMBOL_ITEM_FUNC(netif_to_num) 
    SYMBOL_ITEM_FUNC(snmp_decode_bits) 
    SYMBOL_ITEM_FUNC(snmp_decode_truthvalue) 
    SYMBOL_ITEM_FUNC(snmp_encode_bits) 
    SYMBOL_ITEM_FUNC(snmp_encode_truthvalue) 
    SYMBOL_ITEM_FUNC(snmp_get_device_enterprise_oid) 
    SYMBOL_ITEM_FUNC(snmp_get_next_node_instance_from_oid) 
    SYMBOL_ITEM_FUNC(snmp_get_node_instance_from_oid) 
    SYMBOL_ITEM_FUNC(snmp_ip_port_to_oid) 
    SYMBOL_ITEM_FUNC(snmp_ip_to_oid) 
    SYMBOL_ITEM_FUNC(snmp_ip4_to_oid) 
    SYMBOL_ITEM_FUNC(snmp_ip6_to_oid) 
    SYMBOL_ITEM_FUNC(snmp_mib_tree_resolve_exact) 
    SYMBOL_ITEM_FUNC(snmp_mib_tree_resolve_next) 
    SYMBOL_ITEM_FUNC(snmp_next_oid_check) 
    SYMBOL_ITEM_FUNC(snmp_next_oid_init) 
    SYMBOL_ITEM_FUNC(snmp_next_oid_precheck) 
    SYMBOL_ITEM_FUNC(snmp_oid_append) 
    SYMBOL_ITEM_FUNC(snmp_oid_assign) 
    SYMBOL_ITEM_FUNC(snmp_oid_combine) 
    SYMBOL_ITEM_FUNC(snmp_oid_compare) 
    SYMBOL_ITEM_FUNC(snmp_oid_equal) 
    SYMBOL_ITEM_FUNC(snmp_oid_in_range) 
    SYMBOL_ITEM_FUNC(snmp_oid_prefix) 
    SYMBOL_ITEM_FUNC(snmp_oid_to_ip) 
    SYMBOL_ITEM_FUNC(snmp_oid_to_ip_port) 
    SYMBOL_ITEM_FUNC(snmp_oid_to_ip4) 
    SYMBOL_ITEM_FUNC(snmp_oid_to_ip6) 
    SYMBOL_ITEM_FUNC(snmp_set_device_enterprise_oid) 
    SYMBOL_ITEM_FUNC(snmp_set_mibs) 
    SYMBOL_ITEM_FUNC(snmp_set_test_ok) 
    SYMBOL_ITEM_FUNC(snmp_mib2_lwip_synchronizer) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_syscontact) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_syscontact_readonly) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_sysdescr) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_syslocation) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_syslocation_readonly) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_sysname) 
    SYMBOL_ITEM_FUNC(snmp_mib2_set_sysname_readonly) 
    SYMBOL_ITEM_FUNC(snmp_append_outbound_varbind) 
    SYMBOL_ITEM_FUNC(snmp_get_community) 
    SYMBOL_ITEM_FUNC(snmp_get_community_trap) 
    SYMBOL_ITEM_FUNC(snmp_get_community_write) 
    SYMBOL_ITEM_FUNC(snmp_receive) 
    SYMBOL_ITEM_FUNC(snmp_set_community) 
    SYMBOL_ITEM_FUNC(snmp_set_community_trap) 
    SYMBOL_ITEM_FUNC(snmp_set_community_write) 
    SYMBOL_ITEM_FUNC(snmp_set_inform_callback) 
    SYMBOL_ITEM_FUNC(snmp_set_write_callback) 
    SYMBOL_ITEM_FUNC(snmp_v1_enable) 
    SYMBOL_ITEM_FUNC(snmp_v1_enabled) 
    SYMBOL_ITEM_FUNC(snmp_v2c_enable) 
    SYMBOL_ITEM_FUNC(snmp_v2c_enabled) 
    SYMBOL_ITEM_FUNC(snmp_v3_enable) 
    SYMBOL_ITEM_FUNC(snmp_v3_enabled) 
    SYMBOL_ITEM_FUNC(snmp_varbind_length) 
    SYMBOL_ITEM_FUNC(snmp_vb_enumerator_get_next) 
    SYMBOL_ITEM_FUNC(snmp_vb_enumerator_init) 
    SYMBOL_ITEM_FUNC(snmp_bind_if) 
    SYMBOL_ITEM_FUNC(snmp_get_local_ip_for_dst) 
    SYMBOL_ITEM_FUNC(snmp_init) 
    SYMBOL_ITEM_FUNC(snmp_sendto) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_init) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_read) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_seek) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_seek_abs) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_write) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_writebuf) 
    SYMBOL_ITEM_FUNC(snmp_pbuf_stream_writeto) 
    SYMBOL_ITEM_FUNC(snmp_scalar_array_get_instance) 
    SYMBOL_ITEM_FUNC(snmp_scalar_array_get_next_instance) 
    SYMBOL_ITEM_FUNC(snmp_scalar_get_instance) 
    SYMBOL_ITEM_FUNC(snmp_scalar_get_next_instance) 
    SYMBOL_ITEM_FUNC(snmp_table_extract_value_from_refconstptr) 
    SYMBOL_ITEM_FUNC(snmp_table_extract_value_from_s32ref) 
    SYMBOL_ITEM_FUNC(snmp_table_extract_value_from_u32ref) 
    SYMBOL_ITEM_FUNC(snmp_table_get_instance) 
    SYMBOL_ITEM_FUNC(snmp_table_get_next_instance) 
    SYMBOL_ITEM_FUNC(snmp_table_simple_get_instance) 
    SYMBOL_ITEM_FUNC(snmp_table_simple_get_next_instance) 
    SYMBOL_ITEM_FUNC(snmp_threadsync_get_instance) 
    SYMBOL_ITEM_FUNC(snmp_threadsync_get_next_instance) 
    SYMBOL_ITEM_FUNC(snmp_threadsync_init) 
    SYMBOL_ITEM_FUNC(snmp_authfail_trap) 
    SYMBOL_ITEM_FUNC(snmp_coldstart_trap) 
    SYMBOL_ITEM_FUNC(snmp_get_auth_traps_enabled) 
    SYMBOL_ITEM_FUNC(snmp_get_default_trap_version) 
    SYMBOL_ITEM_FUNC(snmp_send_inform) 
    SYMBOL_ITEM_FUNC(snmp_send_inform_generic) 
    SYMBOL_ITEM_FUNC(snmp_send_inform_specific) 
    SYMBOL_ITEM_FUNC(snmp_send_trap) 
    SYMBOL_ITEM_FUNC(snmp_send_trap_generic) 
    SYMBOL_ITEM_FUNC(snmp_send_trap_specific) 
    SYMBOL_ITEM_FUNC(snmp_set_auth_traps_enabled) 
    SYMBOL_ITEM_FUNC(snmp_set_default_trap_version) 
    SYMBOL_ITEM_FUNC(snmp_trap_dst_enable) 
    SYMBOL_ITEM_FUNC(snmp_trap_dst_ip_set) 
    SYMBOL_ITEM_FUNC(API_INetTelnetBindDev) 
    SYMBOL_ITEM_FUNC(API_INetTelnetInit) 
    SYMBOL_ITEM_FUNC(__netVlanInit) 
    SYMBOL_ITEM_FUNC(API_VlanGet) 
    SYMBOL_ITEM_FUNC(API_VlanSet) 
    SYMBOL_ITEM_FUNC(API_VlanShow) 
    SYMBOL_ITEM_FUNC(API_INetVpnInit) 
    SYMBOL_ITEM_FUNC(__unix_have_event) 
    SYMBOL_ITEM_FUNC(__unix_set_sockfile) 
    SYMBOL_ITEM_FUNC(unix_accept) 
    SYMBOL_ITEM_FUNC(unix_bind) 
    SYMBOL_ITEM_FUNC(unix_close) 
    SYMBOL_ITEM_FUNC(unix_connect) 
    SYMBOL_ITEM_FUNC(unix_connect2) 
    SYMBOL_ITEM_FUNC(unix_getpeername) 
    SYMBOL_ITEM_FUNC(unix_getsockname) 
    SYMBOL_ITEM_FUNC(unix_getsockopt) 
    SYMBOL_ITEM_FUNC(unix_init) 
    SYMBOL_ITEM_FUNC(unix_ioctl) 
    SYMBOL_ITEM_FUNC(unix_listen) 
    SYMBOL_ITEM_FUNC(unix_recv) 
    SYMBOL_ITEM_FUNC(unix_recvfrom) 
    SYMBOL_ITEM_FUNC(unix_recvmsg) 
    SYMBOL_ITEM_FUNC(unix_send) 
    SYMBOL_ITEM_FUNC(unix_sendmsg) 
    SYMBOL_ITEM_FUNC(unix_sendto) 
    SYMBOL_ITEM_FUNC(unix_setsockopt) 
    SYMBOL_ITEM_FUNC(unix_shutdown) 
    SYMBOL_ITEM_FUNC(unix_socket) 
    SYMBOL_ITEM_FUNC(unix_traversal) 
    SYMBOL_ITEM_FUNC(__unix_rmsg_proc) 
    SYMBOL_ITEM_FUNC(__unix_smsg_proc) 
    SYMBOL_ITEM_FUNC(__unix_smsg_unproc) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_check_hook) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_get) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_set) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_set_hook) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_total) 
    SYMBOL_ITEM_FUNC(ethernet_vlan_traversal) 
    SYMBOL_ITEM_FUNC(call_commit_handler) 
    SYMBOL_ITEM_FUNC(get_wireless_stats) 
    SYMBOL_ITEM_FUNC(iw_handler_get_iwstats) 
    SYMBOL_ITEM_FUNC(wext_handle_ioctl) 
    SYMBOL_ITEM_FUNC(wireless_send_event) 
    SYMBOL_ITEM_FUNC(ioctl_private_call) 
    SYMBOL_ITEM_FUNC(iw_handler_get_private) 
    SYMBOL_ITEM_FUNC(iw_handler_get_spy) 
    SYMBOL_ITEM_FUNC(iw_handler_get_thrspy) 
    SYMBOL_ITEM_FUNC(iw_handler_set_spy) 
    SYMBOL_ITEM_FUNC(iw_handler_set_thrspy) 
    SYMBOL_ITEM_FUNC(wireless_spy_update) 
    SYMBOL_ITEM_FUNC(aio_cancel) 
    SYMBOL_ITEM_FUNC(aio_error) 
    SYMBOL_ITEM_FUNC(aio_fsync) 
    SYMBOL_ITEM_FUNC(aio_read) 
    SYMBOL_ITEM_FUNC(aio_return) 
    SYMBOL_ITEM_FUNC(aio_suspend) 
    SYMBOL_ITEM_FUNC(aio_write) 
    SYMBOL_ITEM_FUNC(lio_listio) 
    SYMBOL_ITEM_FUNC(__aioAttachWait) 
    SYMBOL_ITEM_FUNC(__aioAttachWaitNoCheck) 
    SYMBOL_ITEM_FUNC(__aioCancelFd) 
    SYMBOL_ITEM_FUNC(__aioCreateWaitChain) 
    SYMBOL_ITEM_FUNC(__aioDeleteWaitChain) 
    SYMBOL_ITEM_FUNC(__aioDetachWait) 
    SYMBOL_ITEM_FUNC(__aioEnqueue) 
    SYMBOL_ITEM_FUNC(__aioRemoveAiocb) 
    SYMBOL_ITEM_FUNC(__aioSearchFd) 
    SYMBOL_ITEM_FUNC(__aioSigevent) 
    SYMBOL_ITEM_FUNC(__aioWaitChainSetCnt) 
    SYMBOL_ITEM_FUNC(_posixAioInit) 
    SYMBOL_ITEM_FUNC(aio_getstacksize) 
    SYMBOL_ITEM_FUNC(aio_setstacksize) 
    SYMBOL_ITEM_FUNC(dladdr) 
    SYMBOL_ITEM_FUNC(dlclose) 
    SYMBOL_ITEM_FUNC(dlerror) 
    SYMBOL_ITEM_FUNC(dlopen) 
    SYMBOL_ITEM_FUNC(dlrefresh) 
    SYMBOL_ITEM_FUNC(dlsym) 
    SYMBOL_ITEM_FUNC(backtrace_symbols) 
    SYMBOL_ITEM_FUNC(backtrace_symbols_fd) 
    SYMBOL_ITEM_FUNC(fmtmsg) 
    SYMBOL_ITEM_FUNC(fnmatch) 
    SYMBOL_ITEM_FUNC(global_std_get) 
    SYMBOL_ITEM_FUNC(global_std_set) 
    SYMBOL_ITEM_FUNC(register_device) 
    SYMBOL_ITEM_FUNC(register_driver) 
    SYMBOL_ITEM_FUNC(unregister_device) 
    SYMBOL_ITEM_FUNC(unregister_driver) 
    SYMBOL_ITEM_FUNC(gjb_cat) 
    SYMBOL_ITEM_FUNC(gjb_format) 
    SYMBOL_ITEM_FUNC(gjb_mount) 
    SYMBOL_ITEM_FUNC(gjb_umount) 
    SYMBOL_ITEM_FUNC(int_disable_pic) 
    SYMBOL_ITEM_FUNC(int_enable_pic) 
    SYMBOL_ITEM_FUNC(int_lock) 
    SYMBOL_ITEM_FUNC(int_unlock) 
    SYMBOL_ITEM_FUNC(heap_mem_init) 
    SYMBOL_ITEM_FUNC(mem_findmax) 
    SYMBOL_ITEM_FUNC(mem_getinfo) 
    SYMBOL_ITEM_FUNC(mem_show) 
    SYMBOL_ITEM_FUNC(mpart_addmem) 
    SYMBOL_ITEM_FUNC(mpart_alloc) 
    SYMBOL_ITEM_FUNC(mpart_create) 
    SYMBOL_ITEM_FUNC(mpart_delete) 
    SYMBOL_ITEM_FUNC(mpart_findmaxfree) 
    SYMBOL_ITEM_FUNC(mpart_free) 
    SYMBOL_ITEM_FUNC(mpart_getinfo) 
    SYMBOL_ITEM_FUNC(mpart_memalign) 
    SYMBOL_ITEM_FUNC(mpart_module_init) 
    SYMBOL_ITEM_FUNC(mpart_realloc) 
    SYMBOL_ITEM_FUNC(if_show) 
    SYMBOL_ITEM_FUNC(ifconfig) 
    SYMBOL_ITEM_FUNC(mbuf_show) 
    SYMBOL_ITEM_FUNC(routes_show) 
    SYMBOL_ITEM_FUNC(tcp_show) 
    SYMBOL_ITEM_FUNC(udp_show) 
    SYMBOL_ITEM_FUNC(sys_clk_rate_get) 
    SYMBOL_ITEM_FUNC(sys_clk_rate_set) 
    SYMBOL_ITEM_FUNC(tick_get) 
    SYMBOL_ITEM_FUNC(tick_set) 
    SYMBOL_ITEM_FUNC(timer_getinfo) 
    SYMBOL_ITEM_FUNC(wdg_cancel) 
    SYMBOL_ITEM_FUNC(wdg_create) 
    SYMBOL_ITEM_FUNC(wdg_delete) 
    SYMBOL_ITEM_FUNC(wdg_getinfo) 
    SYMBOL_ITEM_FUNC(wdg_show) 
    SYMBOL_ITEM_FUNC(wdg_start) 
    SYMBOL_ITEM_FUNC(mlock) 
    SYMBOL_ITEM_FUNC(mlockall) 
    SYMBOL_ITEM_FUNC(mmap) 
    SYMBOL_ITEM_FUNC(mmap64) 
    SYMBOL_ITEM_FUNC(mprotect) 
    SYMBOL_ITEM_FUNC(mremap) 
    SYMBOL_ITEM_FUNC(msync) 
    SYMBOL_ITEM_FUNC(munlock) 
    SYMBOL_ITEM_FUNC(munlockall) 
    SYMBOL_ITEM_FUNC(munmap) 
    SYMBOL_ITEM_FUNC(posix_madvise) 
    SYMBOL_ITEM_FUNC(shm_open) 
    SYMBOL_ITEM_FUNC(shm_unlink) 
    SYMBOL_ITEM_FUNC(mq_close) 
    SYMBOL_ITEM_FUNC(mq_create) 
    SYMBOL_ITEM_FUNC(mq_delete) 
    SYMBOL_ITEM_FUNC(mq_getattr) 
    SYMBOL_ITEM_FUNC(mq_getinfo) 
    SYMBOL_ITEM_FUNC(mq_notify) 
    SYMBOL_ITEM_FUNC(mq_open) 
    SYMBOL_ITEM_FUNC(mq_open_method) 
    SYMBOL_ITEM_FUNC(mq_receive) 
    SYMBOL_ITEM_FUNC(mq_reltimedreceive_np) 
    SYMBOL_ITEM_FUNC(mq_reltimedsend_np) 
    SYMBOL_ITEM_FUNC(mq_send) 
    SYMBOL_ITEM_FUNC(mq_setattr) 
    SYMBOL_ITEM_FUNC(mq_show) 
    SYMBOL_ITEM_FUNC(mq_timedreceive) 
    SYMBOL_ITEM_FUNC(mq_timedsend) 
    SYMBOL_ITEM_FUNC(mq_unlink) 
    SYMBOL_ITEM_FUNC(poll) 
    SYMBOL_ITEM_FUNC(ppoll) 
    SYMBOL_ITEM_FUNC(__pxnameAdd) 
    SYMBOL_ITEM_FUNC(__pxnameDel) 
    SYMBOL_ITEM_FUNC(__pxnameDelByNode) 
    SYMBOL_ITEM_FUNC(__pxnameGet) 
    SYMBOL_ITEM_FUNC(__pxnameSeach) 
    SYMBOL_ITEM_FUNC(_posixCtxGet) 
    SYMBOL_ITEM_FUNC(_posixCtxTryGet) 
    SYMBOL_ITEM_FUNC(_posixVprocCtxGet) 
    SYMBOL_ITEM_FUNC(API_PosixInit) 
    SYMBOL_ITEM_FUNC(__tshellPosixInit) 
    SYMBOL_ITEM_FUNC(__procFsPosixInfoInit) 
    SYMBOL_ITEM_FUNC(pthread_atfork) 
    SYMBOL_ITEM_FUNC(pthread_cancel) 
    SYMBOL_ITEM_FUNC(pthread_cancelforce) 
    SYMBOL_ITEM_FUNC(pthread_cleanup_pop) 
    SYMBOL_ITEM_FUNC(pthread_cleanup_push) 
    SYMBOL_ITEM_FUNC(pthread_close_hook_add) 
    SYMBOL_ITEM_FUNC(pthread_close_hook_delete) 
    SYMBOL_ITEM_FUNC(pthread_create) 
    SYMBOL_ITEM_FUNC(pthread_create_hook_add) 
    SYMBOL_ITEM_FUNC(pthread_create_hook_delete) 
    SYMBOL_ITEM_FUNC(pthread_delay) 
    SYMBOL_ITEM_FUNC(pthread_detach) 
    SYMBOL_ITEM_FUNC(pthread_equal) 
    SYMBOL_ITEM_FUNC(pthread_exit) 
    SYMBOL_ITEM_FUNC(pthread_getaffinity_np) 
    SYMBOL_ITEM_FUNC(pthread_getconcurrency) 
    SYMBOL_ITEM_FUNC(pthread_getcpuclockid) 
    SYMBOL_ITEM_FUNC(pthread_getid) 
    SYMBOL_ITEM_FUNC(pthread_getinfo) 
    SYMBOL_ITEM_FUNC(pthread_getname) 
    SYMBOL_ITEM_FUNC(pthread_getname_np) 
    SYMBOL_ITEM_FUNC(pthread_getregs) 
    SYMBOL_ITEM_FUNC(pthread_getschedparam) 
    SYMBOL_ITEM_FUNC(pthread_getschedprio) 
    SYMBOL_ITEM_FUNC(pthread_int_lock_np) 
    SYMBOL_ITEM_FUNC(pthread_int_unlock_np) 
    SYMBOL_ITEM_FUNC(pthread_is_ready) 
    SYMBOL_ITEM_FUNC(pthread_is_suspend) 
    SYMBOL_ITEM_FUNC(pthread_join) 
    SYMBOL_ITEM_FUNC(pthread_kill) 
    SYMBOL_ITEM_FUNC(pthread_list_np) 
    SYMBOL_ITEM_FUNC(pthread_lock) 
    SYMBOL_ITEM_FUNC(pthread_null_attr_method_np) 
    SYMBOL_ITEM_FUNC(pthread_once) 
    SYMBOL_ITEM_FUNC(pthread_resume) 
    SYMBOL_ITEM_FUNC(pthread_safe_np) 
    SYMBOL_ITEM_FUNC(pthread_self) 
    SYMBOL_ITEM_FUNC(pthread_setaffinity_np) 
    SYMBOL_ITEM_FUNC(pthread_setcancelstate) 
    SYMBOL_ITEM_FUNC(pthread_setcanceltype) 
    SYMBOL_ITEM_FUNC(pthread_setconcurrency) 
    SYMBOL_ITEM_FUNC(pthread_setname_np) 
    SYMBOL_ITEM_FUNC(pthread_setschedparam) 
    SYMBOL_ITEM_FUNC(pthread_setschedprio) 
    SYMBOL_ITEM_FUNC(pthread_show) 
    SYMBOL_ITEM_FUNC(pthread_showstack) 
    SYMBOL_ITEM_FUNC(pthread_showstackframe) 
    SYMBOL_ITEM_FUNC(pthread_sigmask) 
    SYMBOL_ITEM_FUNC(pthread_start_np) 
    SYMBOL_ITEM_FUNC(pthread_suspend) 
    SYMBOL_ITEM_FUNC(pthread_switch_hook_add) 
    SYMBOL_ITEM_FUNC(pthread_switch_hook_delete) 
    SYMBOL_ITEM_FUNC(pthread_testcancel) 
    SYMBOL_ITEM_FUNC(pthread_unlock) 
    SYMBOL_ITEM_FUNC(pthread_unsafe_np) 
    SYMBOL_ITEM_FUNC(pthread_verify_np) 
    SYMBOL_ITEM_FUNC(pthread_verifyid) 
    SYMBOL_ITEM_FUNC(pthread_wakeup_np) 
    SYMBOL_ITEM_FUNC(pthread_yield) 
    SYMBOL_ITEM_FUNC(pthread_attr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_attr_get_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_getbreakallowed) 
    SYMBOL_ITEM_FUNC(pthread_attr_getdetachstate) 
    SYMBOL_ITEM_FUNC(pthread_attr_getfpallowed) 
    SYMBOL_ITEM_FUNC(pthread_attr_getguardsize) 
    SYMBOL_ITEM_FUNC(pthread_attr_getinheritsched) 
    SYMBOL_ITEM_FUNC(pthread_attr_getinitonly_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_getname) 
    SYMBOL_ITEM_FUNC(pthread_attr_getschedparam) 
    SYMBOL_ITEM_FUNC(pthread_attr_getschedpolicy) 
    SYMBOL_ITEM_FUNC(pthread_attr_getscope) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstack) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstackaddr) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstackfilled) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstacksize) 
    SYMBOL_ITEM_FUNC(pthread_attr_init) 
    SYMBOL_ITEM_FUNC(pthread_attr_setbreakallowed) 
    SYMBOL_ITEM_FUNC(pthread_attr_setdetachstate) 
    SYMBOL_ITEM_FUNC(pthread_attr_setfpallowed) 
    SYMBOL_ITEM_FUNC(pthread_attr_setguardsize) 
    SYMBOL_ITEM_FUNC(pthread_attr_setinheritsched) 
    SYMBOL_ITEM_FUNC(pthread_attr_setinitonly_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_setname) 
    SYMBOL_ITEM_FUNC(pthread_attr_setschedparam) 
    SYMBOL_ITEM_FUNC(pthread_attr_setschedpolicy) 
    SYMBOL_ITEM_FUNC(pthread_attr_setscope) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstack) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstackaddr) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstackfilled) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstacksize) 
    SYMBOL_ITEM_FUNC(pthread_getattr_np) 
    SYMBOL_ITEM_FUNC(pthread_barrier_destroy) 
    SYMBOL_ITEM_FUNC(pthread_barrier_init) 
    SYMBOL_ITEM_FUNC(pthread_barrier_wait) 
    SYMBOL_ITEM_FUNC(pthread_barrierattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_barrierattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_barrierattr_init) 
    SYMBOL_ITEM_FUNC(pthread_barrierattr_setpshared) 
    SYMBOL_ITEM_FUNC(_posixPCondInit) 
    SYMBOL_ITEM_FUNC(pthread_cond_broadcast) 
    SYMBOL_ITEM_FUNC(pthread_cond_destroy) 
    SYMBOL_ITEM_FUNC(pthread_cond_getinfo) 
    SYMBOL_ITEM_FUNC(pthread_cond_init) 
    SYMBOL_ITEM_FUNC(pthread_cond_reltimedwait_np) 
    SYMBOL_ITEM_FUNC(pthread_cond_show) 
    SYMBOL_ITEM_FUNC(pthread_cond_signal) 
    SYMBOL_ITEM_FUNC(pthread_cond_timedwait) 
    SYMBOL_ITEM_FUNC(pthread_cond_wait) 
    SYMBOL_ITEM_FUNC(pthread_condattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_condattr_getclock) 
    SYMBOL_ITEM_FUNC(pthread_condattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_condattr_init) 
    SYMBOL_ITEM_FUNC(pthread_condattr_setclock) 
    SYMBOL_ITEM_FUNC(pthread_condattr_setpshared) 
    SYMBOL_ITEM_FUNC(_PthreadKeyCleanup) 
    SYMBOL_ITEM_FUNC(pthread_getspecific) 
    SYMBOL_ITEM_FUNC(pthread_key_cleanup_np) 
    SYMBOL_ITEM_FUNC(pthread_key_create) 
    SYMBOL_ITEM_FUNC(pthread_key_delete) 
    SYMBOL_ITEM_FUNC(pthread_setspecific) 
    SYMBOL_ITEM_FUNC(__pthread_mutex_init_invisible) 
    SYMBOL_ITEM_FUNC(_posixPMutexInit) 
    SYMBOL_ITEM_FUNC(pthread_mutex_destroy) 
    SYMBOL_ITEM_FUNC(pthread_mutex_getinfo) 
    SYMBOL_ITEM_FUNC(pthread_mutex_getprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutex_init) 
    SYMBOL_ITEM_FUNC(pthread_mutex_lock) 
    SYMBOL_ITEM_FUNC(pthread_mutex_reltimedlock_np) 
    SYMBOL_ITEM_FUNC(pthread_mutex_setprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutex_show) 
    SYMBOL_ITEM_FUNC(pthread_mutex_timedlock) 
    SYMBOL_ITEM_FUNC(pthread_mutex_trylock) 
    SYMBOL_ITEM_FUNC(pthread_mutex_unlock) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getcancelsafe) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getprotocol) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_gettype) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getwaitqtype) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_init) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setcancelsafe) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setprotocol) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setpshared) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_settype) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setwaitqtype) 
    SYMBOL_ITEM_FUNC(_posixPRWLockInit) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_destroy) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_getinfo) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_init) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_rdlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_reltimedrdlock_np) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_reltimedwrlock_np) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_timedrdlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_timedwrlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_tryrdlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_trywrlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_unlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_wrlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_getkind_np) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_init) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_setkind_np) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_setpshared) 
    SYMBOL_ITEM_FUNC(pthread_spin_destroy) 
    SYMBOL_ITEM_FUNC(pthread_spin_init) 
    SYMBOL_ITEM_FUNC(pthread_spin_lock) 
    SYMBOL_ITEM_FUNC(pthread_spin_lock_irq_np) 
    SYMBOL_ITEM_FUNC(pthread_spin_trylock) 
    SYMBOL_ITEM_FUNC(pthread_spin_trylock_irq_np) 
    SYMBOL_ITEM_FUNC(pthread_spin_unlock) 
    SYMBOL_ITEM_FUNC(pthread_spin_unlock_irq_np) 
    SYMBOL_ITEM_FUNC(getpriority) 
    SYMBOL_ITEM_FUNC(getrlimit) 
    SYMBOL_ITEM_FUNC(getrusage) 
    SYMBOL_ITEM_FUNC(nice) 
    SYMBOL_ITEM_FUNC(setpriority) 
    SYMBOL_ITEM_FUNC(setrlimit) 
    SYMBOL_ITEM_FUNC(sched_cpuaffinity_disable_np) 
    SYMBOL_ITEM_FUNC(sched_cpuaffinity_enable_np) 
    SYMBOL_ITEM_FUNC(sched_cpuaffinity_get_np) 
    SYMBOL_ITEM_FUNC(sched_cpuaffinity_set_np) 
    SYMBOL_ITEM_FUNC(sched_rms_destroy) 
    SYMBOL_ITEM_FUNC(sched_rms_init) 
    SYMBOL_ITEM_FUNC(sched_rms_period) 
    SYMBOL_ITEM_FUNC(sched_get_priority_max) 
    SYMBOL_ITEM_FUNC(sched_get_priority_min) 
    SYMBOL_ITEM_FUNC(sched_getaffinity) 
    SYMBOL_ITEM_FUNC(sched_getparam) 
    SYMBOL_ITEM_FUNC(sched_getscheduler) 
    SYMBOL_ITEM_FUNC(sched_gettimeslice) 
    SYMBOL_ITEM_FUNC(sched_rr_get_interval) 
    SYMBOL_ITEM_FUNC(sched_setaffinity) 
    SYMBOL_ITEM_FUNC(sched_setparam) 
    SYMBOL_ITEM_FUNC(sched_setscheduler) 
    SYMBOL_ITEM_FUNC(sched_settimeslice) 
    SYMBOL_ITEM_FUNC(sched_yield) 
    SYMBOL_ITEM_FUNC(_posixPSemInit) 
    SYMBOL_ITEM_FUNC(sem_close) 
    SYMBOL_ITEM_FUNC(sem_destroy) 
    SYMBOL_ITEM_FUNC(sem_flush) 
    SYMBOL_ITEM_FUNC(sem_getinfo) 
    SYMBOL_ITEM_FUNC(sem_getvalue) 
    SYMBOL_ITEM_FUNC(sem_init) 
    SYMBOL_ITEM_FUNC(sem_open) 
    SYMBOL_ITEM_FUNC(sem_open_method) 
    SYMBOL_ITEM_FUNC(sem_post) 
    SYMBOL_ITEM_FUNC(sem_reltimedwait_np) 
    SYMBOL_ITEM_FUNC(sem_show) 
    SYMBOL_ITEM_FUNC(sem_timedwait) 
    SYMBOL_ITEM_FUNC(sem_trywait) 
    SYMBOL_ITEM_FUNC(sem_unlink) 
    SYMBOL_ITEM_FUNC(sem_wait) 
    SYMBOL_ITEM_FUNC(posix_spawn) 
    SYMBOL_ITEM_FUNC(posix_spawn_file_actions_addclose) 
    SYMBOL_ITEM_FUNC(posix_spawn_file_actions_adddup2) 
    SYMBOL_ITEM_FUNC(posix_spawn_file_actions_addopen) 
    SYMBOL_ITEM_FUNC(posix_spawn_file_actions_destroy) 
    SYMBOL_ITEM_FUNC(posix_spawn_file_actions_init) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_destroy) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getextflags) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getflags) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getopt) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getpgroup) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getschedparam) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getschedpolicy) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getsigdefault) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getsigmask) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_getwd) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_init) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setextflags) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setflags) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setopt) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setpgroup) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setschedparam) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setschedpolicy) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setsigdefault) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setsigmask) 
    SYMBOL_ITEM_FUNC(posix_spawnattr_setwd) 
    SYMBOL_ITEM_FUNC(posix_spawnp) 
    SYMBOL_ITEM_FUNC(fpathconf) 
    SYMBOL_ITEM_FUNC(pathconf) 
    SYMBOL_ITEM_FUNC(sysconf) 
    SYMBOL_ITEM_FUNC(_posixSyslogInit) 
    SYMBOL_ITEM_FUNC(closelog) 
    SYMBOL_ITEM_FUNC(openlog) 
    SYMBOL_ITEM_FUNC(setlogmask) 
    SYMBOL_ITEM_FUNC(setlogmask_r) 
    SYMBOL_ITEM_FUNC(syslog) 
    SYMBOL_ITEM_FUNC(syslog_r) 
    SYMBOL_ITEM_FUNC(adjtime) 
    SYMBOL_ITEM_FUNC(ftime) 
    SYMBOL_ITEM_FUNC(times) 
    SYMBOL_ITEM_FUNC(gethostname) 
    SYMBOL_ITEM_FUNC(sethostname) 
    SYMBOL_ITEM_FUNC(uname) 
    SYMBOL_ITEM_FUNC(__tshellFsCmdInit) 
    SYMBOL_ITEM_FUNC(getopt) 
    SYMBOL_ITEM_FUNC(getopt_long) 
    SYMBOL_ITEM_FUNC(getopt_long_only) 
    SYMBOL_ITEM_FUNC(__tShellOptDeleteHook) 
    SYMBOL_ITEM_FUNC(__tshellOptEMsg) 
    SYMBOL_ITEM_FUNC(__tshellOptNonoptEnd) 
    SYMBOL_ITEM_FUNC(__tshellOptNonoptStart) 
    SYMBOL_ITEM_FUNC(__tshellOptPlace) 
    SYMBOL_ITEM_FUNC(API_TShellOptArg) 
    SYMBOL_ITEM_FUNC(API_TShellOptErr) 
    SYMBOL_ITEM_FUNC(API_TShellOptFree) 
    SYMBOL_ITEM_FUNC(API_TShellOptInd) 
    SYMBOL_ITEM_FUNC(API_TShellOptOpt) 
    SYMBOL_ITEM_FUNC(API_TShellOptReset) 
    SYMBOL_ITEM_FUNC(__hashHorner) 
    SYMBOL_ITEM_FUNC(__tshellHeapCmdInit) 
    SYMBOL_ITEM_FUNC(__tshellModemCmdInit) 
    SYMBOL_ITEM_FUNC(__tshellPerfCmdInit) 
    SYMBOL_ITEM_FUNC(__tshellTarCmdInit) 
    SYMBOL_ITEM_FUNC(API_TShellCreate) 
    SYMBOL_ITEM_FUNC(API_TShellCreateEx) 
    SYMBOL_ITEM_FUNC(API_TShellExec) 
    SYMBOL_ITEM_FUNC(API_TShellExecBg) 
    SYMBOL_ITEM_FUNC(API_TShellFlushCache) 
    SYMBOL_ITEM_FUNC(API_TShellFormatAdd) 
    SYMBOL_ITEM_FUNC(API_TShellGetGrpName) 
    SYMBOL_ITEM_FUNC(API_TShellGetOption) 
    SYMBOL_ITEM_FUNC(API_TShellGetUserHome) 
    SYMBOL_ITEM_FUNC(API_TShellGetUserName) 
    SYMBOL_ITEM_FUNC(API_TShellHelpAdd) 
    SYMBOL_ITEM_FUNC(API_TShellHookSet) 
    SYMBOL_ITEM_FUNC(API_TShellInit) 
    SYMBOL_ITEM_FUNC(API_TShellKeywordAdd) 
    SYMBOL_ITEM_FUNC(API_TShellKeywordAddEx) 
    SYMBOL_ITEM_FUNC(API_TShellLogout) 
    SYMBOL_ITEM_FUNC(API_TShellScrClear) 
    SYMBOL_ITEM_FUNC(API_TShellSetOption) 
    SYMBOL_ITEM_FUNC(API_TShellSetStackSize) 
    SYMBOL_ITEM_FUNC(API_TShellSetTitle) 
    SYMBOL_ITEM_FUNC(API_TShellSigEvent) 
    SYMBOL_ITEM_FUNC(API_TShellStartup) 
    SYMBOL_ITEM_FUNC(API_TShellTermAlert) 
    SYMBOL_ITEM_FUNC(__tshellColorInit) 
    SYMBOL_ITEM_FUNC(API_TShellColorEnd) 
    SYMBOL_ITEM_FUNC(API_TShellColorGet) 
    SYMBOL_ITEM_FUNC(API_TShellColorRefresh) 
    SYMBOL_ITEM_FUNC(API_TShellColorStart) 
    SYMBOL_ITEM_FUNC(API_TShellColorStart2) 
    SYMBOL_ITEM_FUNC(__tshellBgCreateEx) 
    SYMBOL_ITEM_FUNC(__tshellExec) 
    SYMBOL_ITEM_FUNC(__tshellKeywordAdd) 
    SYMBOL_ITEM_FUNC(__tshellKeywordFind) 
    SYMBOL_ITEM_FUNC(__tshellKeywordList) 
    SYMBOL_ITEM_FUNC(__tshellPreTreatedBg) 
    SYMBOL_ITEM_FUNC(__tshellRestartEx) 
    SYMBOL_ITEM_FUNC(__tshellShowPrompt) 
    SYMBOL_ITEM_FUNC(__tshellThread) 
    SYMBOL_ITEM_FUNC(__tshellThreadHook) 
    SYMBOL_ITEM_FUNC(__tshellUndef) 
    SYMBOL_ITEM_FUNC(__tshellHistoryBackup) 
    SYMBOL_ITEM_FUNC(__tshellReadline) 
    SYMBOL_ITEM_FUNC(__tshellReadlineClean) 
    SYMBOL_ITEM_FUNC(__tshellSysCmdInit) 
    SYMBOL_ITEM_FUNC(__tshellSysVarInit) 
    SYMBOL_ITEM_FUNC(__tshellStrConvertVar) 
    SYMBOL_ITEM_FUNC(__tshellStrDelCRLF) 
    SYMBOL_ITEM_FUNC(__tshellStrDelTransChar) 
    SYMBOL_ITEM_FUNC(__tshellStrFormat) 
    SYMBOL_ITEM_FUNC(__tshellStrGetToken) 
    SYMBOL_ITEM_FUNC(__tshellStrKeyword) 
    SYMBOL_ITEM_FUNC(__tshellUserCmdInit) 
    SYMBOL_ITEM_FUNC(__tshellUserAuthen) 
    SYMBOL_ITEM_FUNC(__tshellFlushCache) 
    SYMBOL_ITEM_FUNC(__tshellGetGrpName) 
    SYMBOL_ITEM_FUNC(__tshellGetUserName) 
    SYMBOL_ITEM_FUNC(API_TShellVarDelete) 
    SYMBOL_ITEM_FUNC(API_TShellVarDup) 
    SYMBOL_ITEM_FUNC(API_TShellVarGet) 
    SYMBOL_ITEM_FUNC(API_TShellVarGetNum) 
    SYMBOL_ITEM_FUNC(API_TShellVarGetRt) 
    SYMBOL_ITEM_FUNC(API_TShellVarHookSet) 
    SYMBOL_ITEM_FUNC(API_TShellVarLoad) 
    SYMBOL_ITEM_FUNC(API_TShellVarSave) 
    SYMBOL_ITEM_FUNC(API_TShellVarSet) 
    SYMBOL_ITEM_FUNC(__tshellVarAdd) 
    SYMBOL_ITEM_FUNC(__tshellVarDefine) 
    SYMBOL_ITEM_FUNC(__tshellVarDelete) 
    SYMBOL_ITEM_FUNC(__tshellVarDeleteByName) 
    SYMBOL_ITEM_FUNC(__tshellVarDup) 
    SYMBOL_ITEM_FUNC(__tshellVarFind) 
    SYMBOL_ITEM_FUNC(__tshellVarGet) 
    SYMBOL_ITEM_FUNC(__tshellVarGetRt) 
    SYMBOL_ITEM_FUNC(__tshellVarList) 
    SYMBOL_ITEM_FUNC(__tshellVarLoad) 
    SYMBOL_ITEM_FUNC(__tshellVarNum) 
    SYMBOL_ITEM_FUNC(__tshellVarSave) 
    SYMBOL_ITEM_FUNC(__tshellVarSet) 
    SYMBOL_ITEM_FUNC(__symbolAddBsp) 
    SYMBOL_ITEM_FUNC(__symbolAddSmp) 
    SYMBOL_ITEM_FUNC(__symbolAddLibc) 
    SYMBOL_ITEM_FUNC(__procFsSymbolInit) 
    SYMBOL_ITEM_FUNC(__symbolFindHookSet) 
    SYMBOL_ITEM_FUNC(__symbolTraverseHookSet) 
    SYMBOL_ITEM_FUNC(API_SymbolAdd) 
    SYMBOL_ITEM_FUNC(API_SymbolAddStatic) 
    SYMBOL_ITEM_FUNC(API_SymbolDelete) 
    SYMBOL_ITEM_FUNC(API_SymbolFind) 
    SYMBOL_ITEM_FUNC(API_SymbolInit) 
    SYMBOL_ITEM_FUNC(API_SymbolTraverse) 
    SYMBOL_ITEM_FUNC(__busAdapterCreate) 
    SYMBOL_ITEM_FUNC(__busAdapterDelete) 
    SYMBOL_ITEM_FUNC(__busAdapterGet) 
    SYMBOL_ITEM_FUNC(__busSystemInit) 
    SYMBOL_ITEM_FUNC(API_BusShow) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlCreate) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlFree) 
    SYMBOL_ITEM_FUNC(API_AhciDiskAtaParamGet) 
    SYMBOL_ITEM_FUNC(API_AhciDiskAtapiParamGet) 
    SYMBOL_ITEM_FUNC(API_AhciDiskCommandSend) 
    SYMBOL_ITEM_FUNC(API_AhciDiskTrimSet) 
    SYMBOL_ITEM_FUNC(API_AhciNoDataCommandSend) 
    SYMBOL_ITEM_FUNC(API_AhciSwapBufLe16) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlAdd) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlCountGet) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlDelete) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlDrvDel) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlDrvUpdate) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlHandleGetFromIndex) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlHandleGetFromName) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlHandleGetFromPciArg) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlIndexGet) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlInit) 
    SYMBOL_ITEM_FUNC(API_AhciDevAdd) 
    SYMBOL_ITEM_FUNC(API_AhciDevCountGet) 
    SYMBOL_ITEM_FUNC(API_AhciDevDelete) 
    SYMBOL_ITEM_FUNC(API_AhciDevHandleGet) 
    SYMBOL_ITEM_FUNC(API_AhciDevInit) 
    SYMBOL_ITEM_FUNC(API_AhciDevIoctl) 
    SYMBOL_ITEM_FUNC(API_AhciDrvCtrlAdd) 
    SYMBOL_ITEM_FUNC(API_AhciDrvCtrlDelete) 
    SYMBOL_ITEM_FUNC(API_AhciDrvCtrlFind) 
    SYMBOL_ITEM_FUNC(API_AhciDrvDelete) 
    SYMBOL_ITEM_FUNC(API_AhciDrvHandleGet) 
    SYMBOL_ITEM_FUNC(API_AhciDrvInit) 
    SYMBOL_ITEM_FUNC(API_AhciDrvRegister) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlAhciModeEnable) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlCapGet) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlImpPortGet) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlInfoShow) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlIntConnect) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlRegisterWait) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlRegNameGet) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlReset) 
    SYMBOL_ITEM_FUNC(API_AhciCtrlSssSet) 
    SYMBOL_ITEM_FUNC(API_AhciDriveEngineStop) 
    SYMBOL_ITEM_FUNC(API_AhciDriveFwRevInfoGet) 
    SYMBOL_ITEM_FUNC(API_AhciDriveInfoShow) 
    SYMBOL_ITEM_FUNC(API_AhciDriveModelInfoGet) 
    SYMBOL_ITEM_FUNC(API_AhciDrivePowerUp) 
    SYMBOL_ITEM_FUNC(API_AhciDriveRecvFisStop) 
    SYMBOL_ITEM_FUNC(API_AhciDriveRegNameGet) 
    SYMBOL_ITEM_FUNC(API_AhciDriveRegWait) 
    SYMBOL_ITEM_FUNC(API_AhciDriveSectorCountGet) 
    SYMBOL_ITEM_FUNC(API_AhciDriveSerialInfoGet) 
    SYMBOL_ITEM_FUNC(API_AhciDriveWorkModeNameGet) 
    SYMBOL_ITEM_FUNC(API_AhciApmDisable) 
    SYMBOL_ITEM_FUNC(API_AhciApmEnable) 
    SYMBOL_ITEM_FUNC(API_AhciApmModeGet) 
    SYMBOL_ITEM_FUNC(API_AhciPmActive) 
    SYMBOL_ITEM_FUNC(API_AhciPmPowerModeGet) 
    SYMBOL_ITEM_FUNC(API_AtaCtrlIrqIsr) 
    SYMBOL_ITEM_FUNC(API_AtaCtrlStatusCheck) 
    SYMBOL_ITEM_FUNC(API_AtaDrvInit) 
    SYMBOL_ITEM_FUNC(ataCtrlDelay) 
    SYMBOL_ITEM_FUNC(ataDriveCommandSend) 
    SYMBOL_ITEM_FUNC(ataDriveFwRevInfoGet) 
    SYMBOL_ITEM_FUNC(ataDriveInit) 
    SYMBOL_ITEM_FUNC(ataDriveModelInfoGet) 
    SYMBOL_ITEM_FUNC(ataDriveSerialInfoGet) 
    SYMBOL_ITEM_FUNC(ataDrvInit) 
    SYMBOL_ITEM_FUNC(ataSwapBufLe16) 
    SYMBOL_ITEM_FUNC(__blockIoDevCreate) 
    SYMBOL_ITEM_FUNC(__blockIoDevDelete) 
    SYMBOL_ITEM_FUNC(__blockIoDevFlag) 
    SYMBOL_ITEM_FUNC(__blockIoDevGet) 
    SYMBOL_ITEM_FUNC(__blockIoDevInit) 
    SYMBOL_ITEM_FUNC(__blockIoDevIoctl) 
    SYMBOL_ITEM_FUNC(__blockIoDevIsLogic) 
    SYMBOL_ITEM_FUNC(__blockIoDevRead) 
    SYMBOL_ITEM_FUNC(__blockIoDevReset) 
    SYMBOL_ITEM_FUNC(__blockIoDevStatus) 
    SYMBOL_ITEM_FUNC(__blockIoDevWrite) 
    SYMBOL_ITEM_FUNC(API_BlkRawCreate) 
    SYMBOL_ITEM_FUNC(API_BlkRawCreateEx) 
    SYMBOL_ITEM_FUNC(API_BlkRawDelete) 
    SYMBOL_ITEM_FUNC(API_RamDiskCreate) 
    SYMBOL_ITEM_FUNC(API_RamDiskDelete) 
    SYMBOL_ITEM_FUNC(API_BmsgDevCreate) 
    SYMBOL_ITEM_FUNC(API_BmsgDrvInstall) 
    SYMBOL_ITEM_FUNC(bmsgfd) 
    SYMBOL_ITEM_FUNC(bmsgfd_bind) 
    SYMBOL_ITEM_FUNC(bmsgfd_timeout) 
    SYMBOL_ITEM_FUNC(bmsgfd_unbind) 
    SYMBOL_ITEM_FUNC(API_BuzzerDevCreate) 
    SYMBOL_ITEM_FUNC(API_BuzzerDrvInstall) 
    SYMBOL_ITEM_FUNC(API_CanDevCreate) 
    SYMBOL_ITEM_FUNC(API_CanDevRemove) 
    SYMBOL_ITEM_FUNC(API_CanDrvInstall) 
    SYMBOL_ITEM_FUNC(API_DmaContext) 
    SYMBOL_ITEM_FUNC(API_DmaDrvInstall) 
    SYMBOL_ITEM_FUNC(API_DmaFlush) 
    SYMBOL_ITEM_FUNC(API_DmaGetMaxDataBytes) 
    SYMBOL_ITEM_FUNC(API_DmaJobAdd) 
    SYMBOL_ITEM_FUNC(API_DmaJobNodeNum) 
    SYMBOL_ITEM_FUNC(API_DmaMaxNodeNumGet) 
    SYMBOL_ITEM_FUNC(API_DmaMaxNodeNumSet) 
    SYMBOL_ITEM_FUNC(API_DmaReset) 
    SYMBOL_ITEM_FUNC(_dmaDeleteFromWaitList) 
    SYMBOL_ITEM_FUNC(_dmaGetFirstInWaitList) 
    SYMBOL_ITEM_FUNC(_dmaInit) 
    SYMBOL_ITEM_FUNC(_dmaInsertToWaitList) 
    SYMBOL_ITEM_FUNC(_dmaWaitnodeAlloc) 
    SYMBOL_ITEM_FUNC(_dmaWaitnodeFree) 
    SYMBOL_ITEM_FUNC(API_EventfdDevCreate) 
    SYMBOL_ITEM_FUNC(API_EventfdDrvInstall) 
    SYMBOL_ITEM_FUNC(eventfd) 
    SYMBOL_ITEM_FUNC(eventfd_read) 
    SYMBOL_ITEM_FUNC(eventfd_write) 
    SYMBOL_ITEM_FUNC(API_GpiofdDevCreate) 
    SYMBOL_ITEM_FUNC(API_GpiofdDrvInstall) 
    SYMBOL_ITEM_FUNC(gpiofd) 
    SYMBOL_ITEM_FUNC(gpiofd_read) 
    SYMBOL_ITEM_FUNC(gpiofd_write) 
    SYMBOL_ITEM_FUNC(API_GpioChipAdd) 
    SYMBOL_ITEM_FUNC(API_GpioChipDelete) 
    SYMBOL_ITEM_FUNC(API_GpioChipFind) 
    SYMBOL_ITEM_FUNC(API_GpioClearIrq) 
    SYMBOL_ITEM_FUNC(API_GpioDirectionInput) 
    SYMBOL_ITEM_FUNC(API_GpioDirectionOutput) 
    SYMBOL_ITEM_FUNC(API_GpioFree) 
    SYMBOL_ITEM_FUNC(API_GpioFreeArray) 
    SYMBOL_ITEM_FUNC(API_GpioGetFlags) 
    SYMBOL_ITEM_FUNC(API_GpioGetIrq) 
    SYMBOL_ITEM_FUNC(API_GpioGetValue) 
    SYMBOL_ITEM_FUNC(API_GpioHasDrv) 
    SYMBOL_ITEM_FUNC(API_GpioInit) 
    SYMBOL_ITEM_FUNC(API_GpioIsValid) 
    SYMBOL_ITEM_FUNC(API_GpioOpenDrain) 
    SYMBOL_ITEM_FUNC(API_GpioOpenSource) 
    SYMBOL_ITEM_FUNC(API_GpioRequest) 
    SYMBOL_ITEM_FUNC(API_GpioRequestArray) 
    SYMBOL_ITEM_FUNC(API_GpioRequestOne) 
    SYMBOL_ITEM_FUNC(API_GpioSetDebounce) 
    SYMBOL_ITEM_FUNC(API_GpioSetPull) 
    SYMBOL_ITEM_FUNC(API_GpioSetupIrq) 
    SYMBOL_ITEM_FUNC(API_GpioSetValue) 
    SYMBOL_ITEM_FUNC(API_GpioSvrIrq) 
    SYMBOL_ITEM_FUNC(API_GMemDevAdd) 
    SYMBOL_ITEM_FUNC(API_GMemDrawHLine) 
    SYMBOL_ITEM_FUNC(API_GMemDrawVLine) 
    SYMBOL_ITEM_FUNC(API_GMemFillRect) 
    SYMBOL_ITEM_FUNC(API_GMemGet2D) 
    SYMBOL_ITEM_FUNC(API_GMemGetPalette) 
    SYMBOL_ITEM_FUNC(API_GMemGetPixel) 
    SYMBOL_ITEM_FUNC(API_GMemSetAlpha) 
    SYMBOL_ITEM_FUNC(API_GMemSetColor) 
    SYMBOL_ITEM_FUNC(API_GMemSetPalette) 
    SYMBOL_ITEM_FUNC(API_GMemSetPixel) 
    SYMBOL_ITEM_FUNC(API_HstimerfdDevCreate) 
    SYMBOL_ITEM_FUNC(API_HstimerfdDrvInstall) 
    SYMBOL_ITEM_FUNC(hstimerfd_create) 
    SYMBOL_ITEM_FUNC(hstimerfd_gettime) 
    SYMBOL_ITEM_FUNC(hstimerfd_gettime2) 
    SYMBOL_ITEM_FUNC(hstimerfd_hz) 
    SYMBOL_ITEM_FUNC(hstimerfd_settime) 
    SYMBOL_ITEM_FUNC(hstimerfd_settime2) 
    SYMBOL_ITEM_FUNC(API_RtcDevCreate) 
    SYMBOL_ITEM_FUNC(API_RtcDrvInstall) 
    SYMBOL_ITEM_FUNC(API_RtcGet) 
    SYMBOL_ITEM_FUNC(API_RtcSet) 
    SYMBOL_ITEM_FUNC(API_RtcToRoot) 
    SYMBOL_ITEM_FUNC(API_RtcToSys) 
    SYMBOL_ITEM_FUNC(API_SysToRtc) 
    SYMBOL_ITEM_FUNC(API_I2cAdapterCreate) 
    SYMBOL_ITEM_FUNC(API_I2cAdapterDelete) 
    SYMBOL_ITEM_FUNC(API_I2cAdapterGet) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceCreate) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceCtl) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceDelete) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceMasterRecv) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceMasterSend) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceTransfer) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceUsageDec) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceUsageGet) 
    SYMBOL_ITEM_FUNC(API_I2cDeviceUsageInc) 
    SYMBOL_ITEM_FUNC(API_I2cLibInit) 
    SYMBOL_ITEM_FUNC(API_MemDevCreate) 
    SYMBOL_ITEM_FUNC(API_MemDevCreateDir) 
    SYMBOL_ITEM_FUNC(API_MemDevDelete) 
    SYMBOL_ITEM_FUNC(API_MemDrvInstall) 
    SYMBOL_ITEM_FUNC(API_MiiLibInit) 
    SYMBOL_ITEM_FUNC(API_MiiPhyAdd) 
    SYMBOL_ITEM_FUNC(API_MiiPhyDel) 
    SYMBOL_ITEM_FUNC(API_MiiPhyDiagnostic) 
    SYMBOL_ITEM_FUNC(API_MiiPhyInit) 
    SYMBOL_ITEM_FUNC(API_MiiPhyIsolate) 
    SYMBOL_ITEM_FUNC(API_MiiPhyLinkSet) 
    SYMBOL_ITEM_FUNC(API_MiiPhyLinkStatGet) 
    SYMBOL_ITEM_FUNC(API_MiiPhyModeSet) 
    SYMBOL_ITEM_FUNC(API_MiiPhyMonitorStart) 
    SYMBOL_ITEM_FUNC(API_MiiPhyMonitorStop) 
    SYMBOL_ITEM_FUNC(API_MiiPhyProbe) 
    SYMBOL_ITEM_FUNC(API_MiiPhyScan) 
    SYMBOL_ITEM_FUNC(API_MiiPhyScanOnly) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlCreate) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlFree) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlAdd) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlCountGet) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlDelete) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlHandleGetFromIndex) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlHandleGetFromName) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlHandleGetFromPciArg) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlIndexGet) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlInit) 
    SYMBOL_ITEM_FUNC(__nvmeMonitorThread) 
    SYMBOL_ITEM_FUNC(API_NvmeDevAdd) 
    SYMBOL_ITEM_FUNC(API_NvmeDevCountGet) 
    SYMBOL_ITEM_FUNC(API_NvmeDevDelete) 
    SYMBOL_ITEM_FUNC(API_NvmeDevHandleGet) 
    SYMBOL_ITEM_FUNC(API_NvmeDevInit) 
    SYMBOL_ITEM_FUNC(API_NvmeDrvHandleGet) 
    SYMBOL_ITEM_FUNC(API_NvmeDrvInit) 
    SYMBOL_ITEM_FUNC(API_NvmeDrvRegister) 
    SYMBOL_ITEM_FUNC(API_NvmeCtrlIntConnect) 
    SYMBOL_ITEM_FUNC(API_PciAutoBusReset) 
    SYMBOL_ITEM_FUNC(API_PciAutoBusScan) 
    SYMBOL_ITEM_FUNC(API_PciAutoCtrlInit) 
    SYMBOL_ITEM_FUNC(API_PciAutoCtrlRegionSet) 
    SYMBOL_ITEM_FUNC(API_PciAutoDeviceConfig) 
    SYMBOL_ITEM_FUNC(API_PciAutoDeviceSetup) 
    SYMBOL_ITEM_FUNC(API_PciAutoPostScanBridgeSetup) 
    SYMBOL_ITEM_FUNC(API_PciAutoPreScanBridgeSetup) 
    SYMBOL_ITEM_FUNC(API_PciAutoRegionAlign) 
    SYMBOL_ITEM_FUNC(API_PciAutoRegionAllocate) 
    SYMBOL_ITEM_FUNC(API_PciAutoRegionInit) 
    SYMBOL_ITEM_FUNC(API_PciAutoScan) 
    SYMBOL_ITEM_FUNC(API_PciCapEn) 
    SYMBOL_ITEM_FUNC(API_PciCapFind) 
    SYMBOL_ITEM_FUNC(API_PciCapShow) 
    SYMBOL_ITEM_FUNC(API_PciCapExtFind) 
    SYMBOL_ITEM_FUNC(API_PciCapExtShow) 
    SYMBOL_ITEM_FUNC(API_PciDbGetClassStr) 
    SYMBOL_ITEM_FUNC(API_PciDbGetIdStr) 
    SYMBOL_ITEM_FUNC(API_PciDbInit) 
    SYMBOL_ITEM_FUNC(API_PciDevAdd) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigRead) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigReadByte) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigReadDword) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigReadWord) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigWrite) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigWriteByte) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigWriteDword) 
    SYMBOL_ITEM_FUNC(API_PciDevConfigWriteWord) 
    SYMBOL_ITEM_FUNC(API_PciDevDelete) 
    SYMBOL_ITEM_FUNC(API_PciDevDrvDel) 
    SYMBOL_ITEM_FUNC(API_PciDevDrvUpdate) 
    SYMBOL_ITEM_FUNC(API_PciDevHandleGet) 
    SYMBOL_ITEM_FUNC(API_PciDevInit) 
    SYMBOL_ITEM_FUNC(API_PciDevInterConnect) 
    SYMBOL_ITEM_FUNC(API_PciDevInterDisable) 
    SYMBOL_ITEM_FUNC(API_PciDevInterDisableEx) 
    SYMBOL_ITEM_FUNC(API_PciDevInterDisconnect) 
    SYMBOL_ITEM_FUNC(API_PciDevInterEnable) 
    SYMBOL_ITEM_FUNC(API_PciDevInterMsiGet) 
    SYMBOL_ITEM_FUNC(API_PciDevInterServiceCnt) 
    SYMBOL_ITEM_FUNC(API_PciDevInterVectorGet) 
    SYMBOL_ITEM_FUNC(API_PciDevIntxEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciDevIoRemap) 
    SYMBOL_ITEM_FUNC(API_PciDevIoRemap2) 
    SYMBOL_ITEM_FUNC(API_PciDevIoRemapEx) 
    SYMBOL_ITEM_FUNC(API_PciDevIoRemapEx2) 
    SYMBOL_ITEM_FUNC(API_PciDevIoUnmap) 
    SYMBOL_ITEM_FUNC(API_PciDevListCreate) 
    SYMBOL_ITEM_FUNC(API_PciDevMasterEnable) 
    SYMBOL_ITEM_FUNC(API_PciDevParentHandleGet) 
    SYMBOL_ITEM_FUNC(API_PciDevSetupAll) 
    SYMBOL_ITEM_FUNC(API_PciDrvBindEachDev) 
    SYMBOL_ITEM_FUNC(API_PciDevBindEachDrv) 
    SYMBOL_ITEM_FUNC(API_PciDrvDevAdd) 
    SYMBOL_ITEM_FUNC(API_PciDrvDevDel) 
    SYMBOL_ITEM_FUNC(API_PciDrvDevFind) 
    SYMBOL_ITEM_FUNC(API_PciDrvHandleGet) 
    SYMBOL_ITEM_FUNC(API_PciDrvInit) 
    SYMBOL_ITEM_FUNC(API_PciDrvLoad) 
    SYMBOL_ITEM_FUNC(API_PciDrvRegister) 
    SYMBOL_ITEM_FUNC(API_PciDrvUnregister) 
    SYMBOL_ITEM_FUNC(API_PciConfigBusMaxGet) 
    SYMBOL_ITEM_FUNC(API_PciConfigBusMaxSet) 
    SYMBOL_ITEM_FUNC(API_PciConfigFetch) 
    SYMBOL_ITEM_FUNC(API_PciConfigHandleGet) 
    SYMBOL_ITEM_FUNC(API_PciConfigInByte) 
    SYMBOL_ITEM_FUNC(API_PciConfigIndexGet) 
    SYMBOL_ITEM_FUNC(API_PciConfigInDword) 
    SYMBOL_ITEM_FUNC(API_PciConfigInWord) 
    SYMBOL_ITEM_FUNC(API_PciConfigModifyByte) 
    SYMBOL_ITEM_FUNC(API_PciConfigModifyDword) 
    SYMBOL_ITEM_FUNC(API_PciConfigModifyWord) 
    SYMBOL_ITEM_FUNC(API_PciConfigOutByte) 
    SYMBOL_ITEM_FUNC(API_PciConfigOutDword) 
    SYMBOL_ITEM_FUNC(API_PciConfigOutWord) 
    SYMBOL_ITEM_FUNC(API_PciCtrlCreate) 
    SYMBOL_ITEM_FUNC(API_PciCtrlReset) 
    SYMBOL_ITEM_FUNC(API_PciDevMatchDrv) 
    SYMBOL_ITEM_FUNC(API_PciDevMemConfig) 
    SYMBOL_ITEM_FUNC(API_PciDevMemSizeGet) 
    SYMBOL_ITEM_FUNC(API_PciDevResourceGet) 
    SYMBOL_ITEM_FUNC(API_PciDevSetup) 
    SYMBOL_ITEM_FUNC(API_PciDevStdResourceFind) 
    SYMBOL_ITEM_FUNC(API_PciDevStdResourceGet) 
    SYMBOL_ITEM_FUNC(API_PciFindClass) 
    SYMBOL_ITEM_FUNC(API_PciFindDev) 
    SYMBOL_ITEM_FUNC(API_PciFuncDisable) 
    SYMBOL_ITEM_FUNC(API_PciGetHeader) 
    SYMBOL_ITEM_FUNC(API_PciHeaderTypeGet) 
    SYMBOL_ITEM_FUNC(API_PciInterConnect) 
    SYMBOL_ITEM_FUNC(API_PciInterDisconnect) 
    SYMBOL_ITEM_FUNC(API_PciIntxEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciIntxMaskSupported) 
    SYMBOL_ITEM_FUNC(API_PciIrqGet) 
    SYMBOL_ITEM_FUNC(API_PciIrqMsi) 
    SYMBOL_ITEM_FUNC(API_PciResourceGet) 
    SYMBOL_ITEM_FUNC(API_PciSizeNameGet) 
    SYMBOL_ITEM_FUNC(API_PciSizeNumGet) 
    SYMBOL_ITEM_FUNC(API_PciSpecialCycle) 
    SYMBOL_ITEM_FUNC(API_PciTraversal) 
    SYMBOL_ITEM_FUNC(API_PciTraversalDev) 
    SYMBOL_ITEM_FUNC(API_PciVpdRead) 
    SYMBOL_ITEM_FUNC(API_PciDevMsiEnableGet) 
    SYMBOL_ITEM_FUNC(API_PciDevMsiEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciDevMsiRangeEnable) 
    SYMBOL_ITEM_FUNC(API_PciDevMsiVecCountGet) 
    SYMBOL_ITEM_FUNC(API_PciMsi64BitGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiEnableGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMaskBitGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMaskConvert) 
    SYMBOL_ITEM_FUNC(API_PciMsiMaskGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMaskPosGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMaskSet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMsgRead) 
    SYMBOL_ITEM_FUNC(API_PciMsiMsgWrite) 
    SYMBOL_ITEM_FUNC(API_PciMsiMultiCapGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiMultipleGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiPendingGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiPendingPosGet) 
    SYMBOL_ITEM_FUNC(API_PciMsiPendingSet) 
    SYMBOL_ITEM_FUNC(API_PciMsiVecCountGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixClearSet) 
    SYMBOL_ITEM_FUNC(API_PciDevMsixEnableGet) 
    SYMBOL_ITEM_FUNC(API_PciDevMsixEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciDevMsixRangeEnable) 
    SYMBOL_ITEM_FUNC(API_PciDevMsixVecCountGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixEnableGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixEnableSet) 
    SYMBOL_ITEM_FUNC(API_PciMsixFunctionMaskGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixFunctionMaskSet) 
    SYMBOL_ITEM_FUNC(API_PciMsixMaskGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixMaskSet) 
    SYMBOL_ITEM_FUNC(API_PciMsixMsgRead) 
    SYMBOL_ITEM_FUNC(API_PciMsixMsgWrite) 
    SYMBOL_ITEM_FUNC(API_PciMsixPendingGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixPendingPosGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixTablePosGet) 
    SYMBOL_ITEM_FUNC(API_PciMsixVecCountGet) 
    SYMBOL_ITEM_FUNC(__procFsPciInit) 
    SYMBOL_ITEM_FUNC(API_PciScan) 
    SYMBOL_ITEM_FUNC(API_PciBarShow) 
    SYMBOL_ITEM_FUNC(API_PciBistShow) 
    SYMBOL_ITEM_FUNC(API_PciBusDeviceShow) 
    SYMBOL_ITEM_FUNC(API_PciCapMsiShow) 
    SYMBOL_ITEM_FUNC(API_PciCapPcieShow) 
    SYMBOL_ITEM_FUNC(API_PciCapPcixShow) 
    SYMBOL_ITEM_FUNC(API_PciCommandShow) 
    SYMBOL_ITEM_FUNC(API_PciConfigShow) 
    SYMBOL_ITEM_FUNC(API_PciFindClassShow) 
    SYMBOL_ITEM_FUNC(API_PciFindDevShow) 
    SYMBOL_ITEM_FUNC(API_PciHeaderShow) 
    SYMBOL_ITEM_FUNC(API_PciIdsShow) 
    SYMBOL_ITEM_FUNC(API_PciInterruptShow) 
    SYMBOL_ITEM_FUNC(API_PciReversionShow) 
    SYMBOL_ITEM_FUNC(API_PciStatusShow) 
    SYMBOL_ITEM_FUNC(API_PciCapVpdShow) 
    SYMBOL_ITEM_FUNC(API_PipeDevCreate) 
    SYMBOL_ITEM_FUNC(API_PipeDevDelete) 
    SYMBOL_ITEM_FUNC(API_PipeDrvInstall) 
    SYMBOL_ITEM_FUNC(_PipeClose) 
    SYMBOL_ITEM_FUNC(_PipeIoctl) 
    SYMBOL_ITEM_FUNC(_PipeOpen) 
    SYMBOL_ITEM_FUNC(_PipeRead) 
    SYMBOL_ITEM_FUNC(_PipeWrite) 
    SYMBOL_ITEM_FUNC(API_PtyDevCreate) 
    SYMBOL_ITEM_FUNC(API_PtyDevRemove) 
    SYMBOL_ITEM_FUNC(API_PtyDrvInstall) 
    SYMBOL_ITEM_FUNC(_PtyDeviceClose) 
    SYMBOL_ITEM_FUNC(_PtyDeviceIoctl) 
    SYMBOL_ITEM_FUNC(_PtyDeviceOpen) 
    SYMBOL_ITEM_FUNC(_PtyDeviceRead) 
    SYMBOL_ITEM_FUNC(_PtyDeviceStartup) 
    SYMBOL_ITEM_FUNC(_PtyDeviceWrite) 
    SYMBOL_ITEM_FUNC(_PtyHostClose) 
    SYMBOL_ITEM_FUNC(_PtyHostIoctl) 
    SYMBOL_ITEM_FUNC(_PtyHostOpen) 
    SYMBOL_ITEM_FUNC(_PtyHostRead) 
    SYMBOL_ITEM_FUNC(_PtyHostWrite) 
    SYMBOL_ITEM_FUNC(API_RandDevCreate) 
    SYMBOL_ITEM_FUNC(API_RandDrvInstall) 
    SYMBOL_ITEM_FUNC(__randClose) 
    SYMBOL_ITEM_FUNC(__randInit) 
    SYMBOL_ITEM_FUNC(__randIoctl) 
    SYMBOL_ITEM_FUNC(__randOpen) 
    SYMBOL_ITEM_FUNC(__randRead) 
    SYMBOL_ITEM_FUNC(__randWrite) 
    SYMBOL_ITEM_FUNC(API_SdAdapterCreate) 
    SYMBOL_ITEM_FUNC(API_SdAdapterDelete) 
    SYMBOL_ITEM_FUNC(API_SdAdapterGet) 
    SYMBOL_ITEM_FUNC(API_SdDeviceCreate) 
    SYMBOL_ITEM_FUNC(API_SdDeviceCtl) 
    SYMBOL_ITEM_FUNC(API_SdDeviceDelete) 
    SYMBOL_ITEM_FUNC(API_SdDeviceGet) 
    SYMBOL_ITEM_FUNC(API_SdDeviceTransfer) 
    SYMBOL_ITEM_FUNC(API_SdDeviceUsageDec) 
    SYMBOL_ITEM_FUNC(API_SdDeviceUsageGet) 
    SYMBOL_ITEM_FUNC(API_SdDeviceUsageInc) 
    SYMBOL_ITEM_FUNC(API_SdLibInit) 
    SYMBOL_ITEM_FUNC(__sdiobaseDevIrqHandle) 
    SYMBOL_ITEM_FUNC(API_SdioBaseDrvInstall) 
    SYMBOL_ITEM_FUNC(API_SdMemDevCreate) 
    SYMBOL_ITEM_FUNC(API_SdMemDevDelete) 
    SYMBOL_ITEM_FUNC(API_SdMemDevShow) 
    SYMBOL_ITEM_FUNC(API_SdMemDrvInstall) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevAdapterName) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevAppCmd) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevAppSwitch) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCidSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCidView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCmd) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCreate) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCsdSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCsdView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevCtl) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevDelete) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevRcaSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevRcaView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevScrSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevScrView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevStaView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSwCapSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSwCapView) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevTransfer) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevTypeSet) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevTypeView) 
    SYMBOL_ITEM_FUNC(API_SdCoreSpiCxdFormat) 
    SYMBOL_ITEM_FUNC(API_SdCoreSpiMulWrtStop) 
    SYMBOL_ITEM_FUNC(API_SdCoreSpiRegisterRead) 
    SYMBOL_ITEM_FUNC(API_SdCoreSpiSendIfCond) 
    SYMBOL_ITEM_FUNC(API_SdCoreDecodeCID) 
    SYMBOL_ITEM_FUNC(API_SdCoreDecodeCSD) 
    SYMBOL_ITEM_FUNC(API_SdCoreDecodeExtCSD) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevDeSelect) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevGetStatus) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevIsBlockAddr) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevMmcSetRelativeAddr) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevReset) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSelect) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendAllCID) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendAllCSD) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendAllSCR) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendAppOpCond) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendExtCSD) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendIfCond) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSendRelativeAddr) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSetBlkLen) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSetBlkLenRaw) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSetBusWidth) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSetPreBlkLen) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSpiClkDely) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSpiCrcEn) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSwitch) 
    SYMBOL_ITEM_FUNC(API_SdCoreDevSwitchEx) 
    SYMBOL_ITEM_FUNC(__sdCrc16) 
    SYMBOL_ITEM_FUNC(__sdCrcCmdCrc7) 
    SYMBOL_ITEM_FUNC(API_SdmEventNotify) 
    SYMBOL_ITEM_FUNC(API_SdmHostCapGet) 
    SYMBOL_ITEM_FUNC(API_SdmHostExtOptGet) 
    SYMBOL_ITEM_FUNC(API_SdmHostExtOptSet) 
    SYMBOL_ITEM_FUNC(API_SdmHostGet) 
    SYMBOL_ITEM_FUNC(API_SdmHostInterEn) 
    SYMBOL_ITEM_FUNC(API_SdmHostIsCardWp) 
    SYMBOL_ITEM_FUNC(API_SdmHostRegister) 
    SYMBOL_ITEM_FUNC(API_SdmHostUnRegister) 
    SYMBOL_ITEM_FUNC(API_SdmLibInit) 
    SYMBOL_ITEM_FUNC(API_SdmSdDrvRegister) 
    SYMBOL_ITEM_FUNC(API_SdmSdDrvUnRegister) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncBlkSzSet) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncClean) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncDis) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncEn) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncIntDis) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevFuncIntEn) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevHighSpeedEn) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevReadByte) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevReadCCCR) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevReadCis) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevReadFbr) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevReset) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevRwDirect) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevRwExtend) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevRwExtendX) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevSendIoOpCond) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevWideBusEn) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevWriteByte) 
    SYMBOL_ITEM_FUNC(API_SdioCoreDevWriteThenReadByte) 
    SYMBOL_ITEM_FUNC(__sdmSdioDrvAccessRelease) 
    SYMBOL_ITEM_FUNC(__sdmSdioDrvAccessRequest) 
    SYMBOL_ITEM_FUNC(__sdmSdioDrvHeader) 
    SYMBOL_ITEM_FUNC(API_SdmSdioDrvRegister) 
    SYMBOL_ITEM_FUNC(API_SdmSdioDrvUnRegister) 
    SYMBOL_ITEM_FUNC(API_SdmSdioLibInit) 
    SYMBOL_ITEM_FUNC(__sdUnitGet) 
    SYMBOL_ITEM_FUNC(__sdUnitPoolCreate) 
    SYMBOL_ITEM_FUNC(__sdUnitPoolDelete) 
    SYMBOL_ITEM_FUNC(__sdUnitPut) 
    SYMBOL_ITEM_FUNC(API_SdhciDeviceCheckNotify) 
    SYMBOL_ITEM_FUNC(API_SdhciDeviceUsageDec) 
    SYMBOL_ITEM_FUNC(API_SdhciDeviceUsageGet) 
    SYMBOL_ITEM_FUNC(API_SdhciDeviceUsageInc) 
    SYMBOL_ITEM_FUNC(API_SdhciHostCreate) 
    SYMBOL_ITEM_FUNC(API_SdhciHostDelete) 
    SYMBOL_ITEM_FUNC(API_SdhciHostTransferModGet) 
    SYMBOL_ITEM_FUNC(API_SdhciHostTransferModSet) 
    SYMBOL_ITEM_FUNC(API_SdhciSdmHostGet) 
    SYMBOL_ITEM_FUNC(semfd) 
    SYMBOL_ITEM_FUNC(semfd_timeout) 
    SYMBOL_ITEM_FUNC(API_SemfdDevCreate) 
    SYMBOL_ITEM_FUNC(API_SemfdDrvInstall) 
    SYMBOL_ITEM_FUNC(API_ShmDevCreate) 
    SYMBOL_ITEM_FUNC(API_ShmDrvInstall) 
    SYMBOL_ITEM_FUNC(API_SpiAdapterCreate) 
    SYMBOL_ITEM_FUNC(API_SpiAdapterDelete) 
    SYMBOL_ITEM_FUNC(API_SpiAdapterGet) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceBusRelease) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceBusRequest) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceCreate) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceCtl) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceDelete) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceTransfer) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceUsageDec) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceUsageGet) 
    SYMBOL_ITEM_FUNC(API_SpiDeviceUsageInc) 
    SYMBOL_ITEM_FUNC(API_SpiLibInit) 
    SYMBOL_ITEM_FUNC(API_SpipeDevCreate) 
    SYMBOL_ITEM_FUNC(API_SpipeDevDelete) 
    SYMBOL_ITEM_FUNC(API_SpipeDrvInstall) 
    SYMBOL_ITEM_FUNC(_SpipeClose) 
    SYMBOL_ITEM_FUNC(_SpipeIoctl) 
    SYMBOL_ITEM_FUNC(_SpipeOpen) 
    SYMBOL_ITEM_FUNC(_SpipeRead) 
    SYMBOL_ITEM_FUNC(_SpipeRemove) 
    SYMBOL_ITEM_FUNC(_SpipeWrite) 
    SYMBOL_ITEM_FUNC(cfgetispeed) 
    SYMBOL_ITEM_FUNC(cfgetospeed) 
    SYMBOL_ITEM_FUNC(cfmakeraw) 
    SYMBOL_ITEM_FUNC(cfsetispeed) 
    SYMBOL_ITEM_FUNC(cfsetospeed) 
    SYMBOL_ITEM_FUNC(tcdrain) 
    SYMBOL_ITEM_FUNC(tcflow) 
    SYMBOL_ITEM_FUNC(tcflush) 
    SYMBOL_ITEM_FUNC(tcgetattr) 
    SYMBOL_ITEM_FUNC(tcgetsid) 
    SYMBOL_ITEM_FUNC(tcsendbreak) 
    SYMBOL_ITEM_FUNC(tcsetattr) 
    SYMBOL_ITEM_FUNC(API_TtyDevCreate) 
    SYMBOL_ITEM_FUNC(API_TtyDevRemove) 
    SYMBOL_ITEM_FUNC(API_TtyDrvInstall) 
    SYMBOL_ITEM_FUNC(_TyDevInit) 
    SYMBOL_ITEM_FUNC(_TyDevRemove) 
    SYMBOL_ITEM_FUNC(_TyIoctl) 
    SYMBOL_ITEM_FUNC(_TyIRd) 
    SYMBOL_ITEM_FUNC(_TyITx) 
    SYMBOL_ITEM_FUNC(_TyRead) 
    SYMBOL_ITEM_FUNC(_TyWrite) 
    SYMBOL_ITEM_FUNC(API_TyAbortFuncSet) 
    SYMBOL_ITEM_FUNC(API_TyAbortSet) 
    SYMBOL_ITEM_FUNC(API_TyBackspaceSet) 
    SYMBOL_ITEM_FUNC(API_TyDeleteLineSet) 
    SYMBOL_ITEM_FUNC(API_TyEOFSet) 
    SYMBOL_ITEM_FUNC(API_TyMonitorTrapSet) 
    SYMBOL_ITEM_FUNC(API_SystemHwInfoPrint) 
    SYMBOL_ITEM_FUNC(API_SystemLogoPrint) 
    SYMBOL_ITEM_FUNC(_epollAddEvent) 
    SYMBOL_ITEM_FUNC(_epollDelEvent) 
    SYMBOL_ITEM_FUNC(_epollDevCreate) 
    SYMBOL_ITEM_FUNC(_epollDrvInstall) 
    SYMBOL_ITEM_FUNC(_epollFindEvent) 
    SYMBOL_ITEM_FUNC(_epollFiniFdset) 
    SYMBOL_ITEM_FUNC(_epollInitFdset) 
    SYMBOL_ITEM_FUNC(_epollModEvent) 
    SYMBOL_ITEM_FUNC(_EpollInit) 
    SYMBOL_ITEM_FUNC(epoll_create) 
    SYMBOL_ITEM_FUNC(epoll_create1) 
    SYMBOL_ITEM_FUNC(epoll_ctl) 
    SYMBOL_ITEM_FUNC(epoll_pwait) 
    SYMBOL_ITEM_FUNC(epoll_wait) 
    SYMBOL_ITEM_FUNC(_ExcGetLost) 
    SYMBOL_ITEM_FUNC(_excInit) 
    SYMBOL_ITEM_FUNC(_excJobAdd) 
    SYMBOL_ITEM_FUNC(_excJobDel) 
    SYMBOL_ITEM_FUNC(_hotplugDevCreate) 
    SYMBOL_ITEM_FUNC(_hotplugDevPutMsg) 
    SYMBOL_ITEM_FUNC(_hotplugDrvInstall) 
    SYMBOL_ITEM_FUNC(_hotplugInit) 
    SYMBOL_ITEM_FUNC(API_HotplugContext) 
    SYMBOL_ITEM_FUNC(API_HotplugEvent) 
    SYMBOL_ITEM_FUNC(API_HotplugEventMessage) 
    SYMBOL_ITEM_FUNC(API_HotplugGetLost) 
    SYMBOL_ITEM_FUNC(API_HotplugPollAdd) 
    SYMBOL_ITEM_FUNC(API_HotplugPollDelete) 
    SYMBOL_ITEM_FUNC(closedir) 
    SYMBOL_ITEM_FUNC(dirfd) 
    SYMBOL_ITEM_FUNC(futimes) 
    SYMBOL_ITEM_FUNC(mkdir) 
    SYMBOL_ITEM_FUNC(opendir) 
    SYMBOL_ITEM_FUNC(readdir) 
    SYMBOL_ITEM_FUNC(readdir_r) 
    SYMBOL_ITEM_FUNC(readdir64) 
    SYMBOL_ITEM_FUNC(readdir64_r) 
    SYMBOL_ITEM_FUNC(rewinddir) 
    SYMBOL_ITEM_FUNC(rmdir) 
    SYMBOL_ITEM_FUNC(utime) 
    SYMBOL_ITEM_FUNC(utimes) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(API_IosFdNodeAdd) 
    SYMBOL_ITEM_FUNC(API_IosFdNodeDec) 
    SYMBOL_ITEM_FUNC(API_IosFdNodeFind) 
    SYMBOL_ITEM_FUNC(API_IosFdNodeLock) 
    SYMBOL_ITEM_FUNC(mkfifo) 
    SYMBOL_ITEM_FUNC(pipe) 
    SYMBOL_ITEM_FUNC(pipe2) 
    SYMBOL_ITEM_FUNC(_IosFileClose) 
    SYMBOL_ITEM_FUNC(_IosFileDelete) 
    SYMBOL_ITEM_FUNC(_IosFileDescGet) 
    SYMBOL_ITEM_FUNC(_IosFileDup) 
    SYMBOL_ITEM_FUNC(_IosFileDup2) 
    SYMBOL_ITEM_FUNC(_IosFileGet) 
    SYMBOL_ITEM_FUNC(_IosFileGetKernel) 
    SYMBOL_ITEM_FUNC(_IosFileIoctl) 
    SYMBOL_ITEM_FUNC(_IosFileListIslock) 
    SYMBOL_ITEM_FUNC(_IosFileListLock) 
    SYMBOL_ITEM_FUNC(_IosFileListRemoveReq) 
    SYMBOL_ITEM_FUNC(_IosFileListUnlock) 
    SYMBOL_ITEM_FUNC(_IosFileNew) 
    SYMBOL_ITEM_FUNC(_IosFileRealName) 
    SYMBOL_ITEM_FUNC(_IosFileRefDec) 
    SYMBOL_ITEM_FUNC(_IosFileRefGet) 
    SYMBOL_ITEM_FUNC(_IosFileRefInc) 
    SYMBOL_ITEM_FUNC(_IosFileSet) 
    SYMBOL_ITEM_FUNC(_IosFileSync) 
    SYMBOL_ITEM_FUNC(API_IosFdDevFind) 
    SYMBOL_ITEM_FUNC(API_IosFdEntryReclaim) 
    SYMBOL_ITEM_FUNC(API_IosFdFree) 
    SYMBOL_ITEM_FUNC(API_IosFdGetCloExec) 
    SYMBOL_ITEM_FUNC(API_IosFdGetFlag) 
    SYMBOL_ITEM_FUNC(API_IosFdGetName) 
    SYMBOL_ITEM_FUNC(API_IosFdGetRealName) 
    SYMBOL_ITEM_FUNC(API_IosFdGetType) 
    SYMBOL_ITEM_FUNC(API_IosFdIsBusy) 
    SYMBOL_ITEM_FUNC(API_IosFdLock) 
    SYMBOL_ITEM_FUNC(API_IosFdNew) 
    SYMBOL_ITEM_FUNC(API_IosFdRealName) 
    SYMBOL_ITEM_FUNC(API_IosFdRefDec) 
    SYMBOL_ITEM_FUNC(API_IosFdRefInc) 
    SYMBOL_ITEM_FUNC(API_IosFdSet) 
    SYMBOL_ITEM_FUNC(API_IosFdSetCloExec) 
    SYMBOL_ITEM_FUNC(API_IosFdUnlink) 
    SYMBOL_ITEM_FUNC(API_IosFdValue) 
    SYMBOL_ITEM_FUNC(API_IosFdValueType) 
    SYMBOL_ITEM_FUNC(diskformat) 
    SYMBOL_ITEM_FUNC(diskinit) 
    SYMBOL_ITEM_FUNC(API_IoDefDevGet) 
    SYMBOL_ITEM_FUNC(API_IoDefDirGet) 
    SYMBOL_ITEM_FUNC(API_IoDefPathCat) 
    SYMBOL_ITEM_FUNC(API_IoDefPathGet) 
    SYMBOL_ITEM_FUNC(API_IoDefPathSet) 
    SYMBOL_ITEM_FUNC(API_IoFullFileNameGet) 
    SYMBOL_ITEM_FUNC(API_IoGlobalStdGet) 
    SYMBOL_ITEM_FUNC(API_IoGlobalStdSet) 
    SYMBOL_ITEM_FUNC(API_IoPathCondense) 
    SYMBOL_ITEM_FUNC(API_IoPrivateEnv) 
    SYMBOL_ITEM_FUNC(API_IoTaskStdGet) 
    SYMBOL_ITEM_FUNC(API_IoTaskStdSet) 
    SYMBOL_ITEM_FUNC(cd) 
    SYMBOL_ITEM_FUNC(chdir) 
    SYMBOL_ITEM_FUNC(chroot) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(creat) 
    SYMBOL_ITEM_FUNC(dup) 
    SYMBOL_ITEM_FUNC(dup2) 
    SYMBOL_ITEM_FUNC(dup2kernel) 
    SYMBOL_ITEM_FUNC(dupminfd) 
    SYMBOL_ITEM_FUNC(fchdir) 
    SYMBOL_ITEM_FUNC(fisbusy) 
    SYMBOL_ITEM_FUNC(flock) 
    SYMBOL_ITEM_FUNC(getcwd) 
    SYMBOL_ITEM_FUNC(getwd) 
    SYMBOL_ITEM_FUNC(ioctl) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(llseek) 
    SYMBOL_ITEM_FUNC(lockf) 
    SYMBOL_ITEM_FUNC(lseek) 
    SYMBOL_ITEM_FUNC(lseek64) 
    SYMBOL_ITEM_FUNC(major) 
    SYMBOL_ITEM_FUNC(makedev) 
    SYMBOL_ITEM_FUNC(minor) 
    SYMBOL_ITEM_FUNC(mknod) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(open64) 
    SYMBOL_ITEM_FUNC(pread) 
    SYMBOL_ITEM_FUNC(pread64) 
    SYMBOL_ITEM_FUNC(pwrite) 
    SYMBOL_ITEM_FUNC(pwrite64) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(readv) 
    SYMBOL_ITEM_FUNC(realpath) 
    SYMBOL_ITEM_FUNC(rename) 
    SYMBOL_ITEM_FUNC(ttyname) 
    SYMBOL_ITEM_FUNC(ttyname_r) 
    SYMBOL_ITEM_FUNC(umask) 
    SYMBOL_ITEM_FUNC(unlink) 
    SYMBOL_ITEM_FUNC(write) 
    SYMBOL_ITEM_FUNC(writev) 
    SYMBOL_ITEM_FUNC(_IosCheckPermissions) 
    SYMBOL_ITEM_FUNC(_IosEnvCreate) 
    SYMBOL_ITEM_FUNC(_IosEnvDelete) 
    SYMBOL_ITEM_FUNC(_IosEnvGetDef) 
    SYMBOL_ITEM_FUNC(_IosEnvInherit) 
    SYMBOL_ITEM_FUNC(_IosInit) 
    SYMBOL_ITEM_FUNC(_IosLock) 
    SYMBOL_ITEM_FUNC(_IosUnlock) 
    SYMBOL_ITEM_FUNC(API_IoGetDrvAuthor) 
    SYMBOL_ITEM_FUNC(API_IoGetDrvDescription) 
    SYMBOL_ITEM_FUNC(API_IoGetDrvLicense) 
    SYMBOL_ITEM_FUNC(API_IoSetDrvAuthor) 
    SYMBOL_ITEM_FUNC(API_IoSetDrvDescription) 
    SYMBOL_ITEM_FUNC(API_IoSetDrvLicense) 
    SYMBOL_ITEM_FUNC(__fdLockfCleanupHook) 
    SYMBOL_ITEM_FUNC(_FdLockfClearFdEntry) 
    SYMBOL_ITEM_FUNC(_FdLockfClearFdNode) 
    SYMBOL_ITEM_FUNC(_FdLockfIoctl) 
    SYMBOL_ITEM_FUNC(_FdLockfProc) 
    SYMBOL_ITEM_FUNC(_PathBuildLink) 
    SYMBOL_ITEM_FUNC(_PathCat) 
    SYMBOL_ITEM_FUNC(_PathCondense) 
    SYMBOL_ITEM_FUNC(_PathGetDef) 
    SYMBOL_ITEM_FUNC(_PathGetFull) 
    SYMBOL_ITEM_FUNC(_PathLastName) 
    SYMBOL_ITEM_FUNC(_PathLastNamePtr) 
    SYMBOL_ITEM_FUNC(_PathMoveCheck) 
    SYMBOL_ITEM_FUNC(API_IoDevShow) 
    SYMBOL_ITEM_FUNC(API_IoDrvLicenseShow) 
    SYMBOL_ITEM_FUNC(API_IoDrvShow) 
    SYMBOL_ITEM_FUNC(API_IoFdentryShow) 
    SYMBOL_ITEM_FUNC(API_IoFdShow) 
    SYMBOL_ITEM_FUNC(access) 
    SYMBOL_ITEM_FUNC(chmod) 
    SYMBOL_ITEM_FUNC(chown) 
    SYMBOL_ITEM_FUNC(fchmod) 
    SYMBOL_ITEM_FUNC(fchown) 
    SYMBOL_ITEM_FUNC(fdatasync) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(fstat64) 
    SYMBOL_ITEM_FUNC(fstatfs) 
    SYMBOL_ITEM_FUNC(fsync) 
    SYMBOL_ITEM_FUNC(ftruncate) 
    SYMBOL_ITEM_FUNC(ftruncate64) 
    SYMBOL_ITEM_FUNC(lchown) 
    SYMBOL_ITEM_FUNC(lstat) 
    SYMBOL_ITEM_FUNC(lstat64) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(stat64) 
    SYMBOL_ITEM_FUNC(statfs) 
    SYMBOL_ITEM_FUNC(sync) 
    SYMBOL_ITEM_FUNC(truncate) 
    SYMBOL_ITEM_FUNC(truncate64) 
    SYMBOL_ITEM_FUNC(link) 
    SYMBOL_ITEM_FUNC(readlink) 
    SYMBOL_ITEM_FUNC(symlink) 
    SYMBOL_ITEM_FUNC(API_IosClose) 
    SYMBOL_ITEM_FUNC(API_IosCreate) 
    SYMBOL_ITEM_FUNC(API_IosDelete) 
    SYMBOL_ITEM_FUNC(API_IosDevAdd) 
    SYMBOL_ITEM_FUNC(API_IosDevAddEx) 
    SYMBOL_ITEM_FUNC(API_IosDevDelete) 
    SYMBOL_ITEM_FUNC(API_IosDevFileAbnormal) 
    SYMBOL_ITEM_FUNC(API_IosDevFind) 
    SYMBOL_ITEM_FUNC(API_IosDevMatch) 
    SYMBOL_ITEM_FUNC(API_IosDevMatchFull) 
    SYMBOL_ITEM_FUNC(API_IosDrvGetType) 
    SYMBOL_ITEM_FUNC(API_IosDrvInstall) 
    SYMBOL_ITEM_FUNC(API_IosDrvInstallEx) 
    SYMBOL_ITEM_FUNC(API_IosDrvInstallEx2) 
    SYMBOL_ITEM_FUNC(API_IosDrvRemove) 
    SYMBOL_ITEM_FUNC(API_IosFstat) 
    SYMBOL_ITEM_FUNC(API_IosIoctl) 
    SYMBOL_ITEM_FUNC(API_IosLseek) 
    SYMBOL_ITEM_FUNC(API_IosLstat) 
    SYMBOL_ITEM_FUNC(API_IosMmap) 
    SYMBOL_ITEM_FUNC(API_IosNextDevGet) 
    SYMBOL_ITEM_FUNC(API_IosOpen) 
    SYMBOL_ITEM_FUNC(API_IosPRead) 
    SYMBOL_ITEM_FUNC(API_IosPWrite) 
    SYMBOL_ITEM_FUNC(API_IosRead) 
    SYMBOL_ITEM_FUNC(API_IosReadlink) 
    SYMBOL_ITEM_FUNC(API_IosSymlink) 
    SYMBOL_ITEM_FUNC(API_IosUnmap) 
    SYMBOL_ITEM_FUNC(API_IosWrite) 
    SYMBOL_ITEM_FUNC(__logPrintf) 
    SYMBOL_ITEM_FUNC(__logPrintk) 
    SYMBOL_ITEM_FUNC(_logInit) 
    SYMBOL_ITEM_FUNC(_LogThread) 
    SYMBOL_ITEM_FUNC(API_LogFdAdd) 
    SYMBOL_ITEM_FUNC(API_LogFdDelete) 
    SYMBOL_ITEM_FUNC(API_LogFdGet) 
    SYMBOL_ITEM_FUNC(API_LogFdSet) 
    SYMBOL_ITEM_FUNC(API_LogMsg) 
    SYMBOL_ITEM_FUNC(API_LogPrintk) 
    SYMBOL_ITEM_FUNC(API_PowerMAdapterCreate) 
    SYMBOL_ITEM_FUNC(API_PowerMAdapterDelete) 
    SYMBOL_ITEM_FUNC(API_PowerMAdapterFind) 
    SYMBOL_ITEM_FUNC(API_PowerMDevInit) 
    SYMBOL_ITEM_FUNC(API_PowerMDevOff) 
    SYMBOL_ITEM_FUNC(API_PowerMDevOn) 
    SYMBOL_ITEM_FUNC(API_PowerMDevTerm) 
    SYMBOL_ITEM_FUNC(_PowerMInit) 
    SYMBOL_ITEM_FUNC(API_PowerMDevGetWatchDog) 
    SYMBOL_ITEM_FUNC(API_PowerMDevSetWatchDog) 
    SYMBOL_ITEM_FUNC(API_PowerMDevWatchDogOff) 
    SYMBOL_ITEM_FUNC(API_PowerMCpuGet) 
    SYMBOL_ITEM_FUNC(API_PowerMCpuSet) 
    SYMBOL_ITEM_FUNC(API_PowerMResume) 
    SYMBOL_ITEM_FUNC(API_PowerMSavingEnter) 
    SYMBOL_ITEM_FUNC(API_PowerMSavingExit) 
    SYMBOL_ITEM_FUNC(API_PowerMSuspend) 
    SYMBOL_ITEM_FUNC(alarm) 
    SYMBOL_ITEM_FUNC(getitimer) 
    SYMBOL_ITEM_FUNC(setitimer) 
    SYMBOL_ITEM_FUNC(timer_create) 
    SYMBOL_ITEM_FUNC(timer_create_internal) 
    SYMBOL_ITEM_FUNC(timer_delete) 
    SYMBOL_ITEM_FUNC(timer_getoverrun) 
    SYMBOL_ITEM_FUNC(timer_getoverrun_64) 
    SYMBOL_ITEM_FUNC(timer_gettime) 
    SYMBOL_ITEM_FUNC(timer_setfile) 
    SYMBOL_ITEM_FUNC(timer_settime) 
    SYMBOL_ITEM_FUNC(ualarm) 
    SYMBOL_ITEM_FUNC(_tmrfdCallback) 
    SYMBOL_ITEM_FUNC(API_TimerfdDevCreate) 
    SYMBOL_ITEM_FUNC(API_TimerfdDrvInstall) 
    SYMBOL_ITEM_FUNC(timerfd_create) 
    SYMBOL_ITEM_FUNC(timerfd_gettime) 
    SYMBOL_ITEM_FUNC(timerfd_settime) 
    SYMBOL_ITEM_FUNC(__selDoIoctls) 
    SYMBOL_ITEM_FUNC(_SelectInit) 
    SYMBOL_ITEM_FUNC(__selFdsetInit) 
    SYMBOL_ITEM_FUNC(pselect) 
    SYMBOL_ITEM_FUNC(select) 
    SYMBOL_ITEM_FUNC(select_method) 
    SYMBOL_ITEM_FUNC(API_SelWakeupListInit) 
    SYMBOL_ITEM_FUNC(API_SelWakeupListLen) 
    SYMBOL_ITEM_FUNC(API_SelWakeupListTerm) 
    SYMBOL_ITEM_FUNC(API_SelNodeAdd) 
    SYMBOL_ITEM_FUNC(API_SelNodeDelete) 
    SYMBOL_ITEM_FUNC(API_SelWakeup) 
    SYMBOL_ITEM_FUNC(API_SelWakeupAll) 
    SYMBOL_ITEM_FUNC(API_SelWakeupAllByFlags) 
    SYMBOL_ITEM_FUNC(API_SelWakeupError) 
    SYMBOL_ITEM_FUNC(API_SelWakeupFifo) 
    SYMBOL_ITEM_FUNC(API_SelWakeupPrio) 
    SYMBOL_ITEM_FUNC(API_SelWakeupTerm) 
    SYMBOL_ITEM_FUNC(API_SelWakeupType) 
    SYMBOL_ITEM_FUNC(__selPtyAdd) 
    SYMBOL_ITEM_FUNC(__selPtyDelete) 
    SYMBOL_ITEM_FUNC(__selTyAdd) 
    SYMBOL_ITEM_FUNC(__selTyDelete) 
    SYMBOL_ITEM_FUNC(waitexcept) 
    SYMBOL_ITEM_FUNC(waitread) 
    SYMBOL_ITEM_FUNC(waitwrite) 
    SYMBOL_ITEM_FUNC(bsd_signal) 
    SYMBOL_ITEM_FUNC(kill) 
    SYMBOL_ITEM_FUNC(pause) 
    SYMBOL_ITEM_FUNC(raise) 
    SYMBOL_ITEM_FUNC(sigaction) 
    SYMBOL_ITEM_FUNC(sigaddset) 
    SYMBOL_ITEM_FUNC(sigaltstack) 
    SYMBOL_ITEM_FUNC(sigblock) 
    SYMBOL_ITEM_FUNC(sigdelset) 
    SYMBOL_ITEM_FUNC(sigemptyset) 
    SYMBOL_ITEM_FUNC(sigfillset) 
    SYMBOL_ITEM_FUNC(sigGetAction) 
    SYMBOL_ITEM_FUNC(siggetmask) 
    SYMBOL_ITEM_FUNC(sighold) 
    SYMBOL_ITEM_FUNC(sigignore) 
    SYMBOL_ITEM_FUNC(siginterrupt) 
    SYMBOL_ITEM_FUNC(sigismember) 
    SYMBOL_ITEM_FUNC(sigmask) 
    SYMBOL_ITEM_FUNC(signal) 
    SYMBOL_ITEM_FUNC(sigpause) 
    SYMBOL_ITEM_FUNC(sigpending) 
    SYMBOL_ITEM_FUNC(sigprocmask) 
    SYMBOL_ITEM_FUNC(sigqueue) 
    SYMBOL_ITEM_FUNC(sigrelse) 
    SYMBOL_ITEM_FUNC(sigset) 
    SYMBOL_ITEM_FUNC(sigsetmask) 
    SYMBOL_ITEM_FUNC(sigstack) 
    SYMBOL_ITEM_FUNC(sigsuspend) 
    SYMBOL_ITEM_FUNC(sigtimedwait) 
    SYMBOL_ITEM_FUNC(sigTrap) 
    SYMBOL_ITEM_FUNC(sigvec) 
    SYMBOL_ITEM_FUNC(sigwait) 
    SYMBOL_ITEM_FUNC(sigwaitinfo) 
    SYMBOL_ITEM_FUNC(_sigfdReadUnblock) 
    SYMBOL_ITEM_FUNC(API_SignalfdDevCreate) 
    SYMBOL_ITEM_FUNC(API_SignalfdDrvInstall) 
    SYMBOL_ITEM_FUNC(signalfd) 
    SYMBOL_ITEM_FUNC(__sigEventArgInit) 
    SYMBOL_ITEM_FUNC(_doSigEvent) 
    SYMBOL_ITEM_FUNC(_doSigEventEx) 
    SYMBOL_ITEM_FUNC(__siglongjmpSetup) 
    SYMBOL_ITEM_FUNC(__sigsetjmpSetup) 
    SYMBOL_ITEM_FUNC(_doKill) 
    SYMBOL_ITEM_FUNC(_doSignal) 
    SYMBOL_ITEM_FUNC(_doSigQueue) 
    SYMBOL_ITEM_FUNC(_sigGetAction) 
    SYMBOL_ITEM_FUNC(_signalGetCtx) 
    SYMBOL_ITEM_FUNC(_signalInit) 
    SYMBOL_ITEM_FUNC(_sigPendFree) 
    SYMBOL_ITEM_FUNC(_sigPendGet) 
    SYMBOL_ITEM_FUNC(_sigPendInit) 
    SYMBOL_ITEM_FUNC(_sigPendRun) 
    SYMBOL_ITEM_FUNC(_sigTimeoutRecalc) 
    SYMBOL_ITEM_FUNC(API_SystemHookAdd) 
    SYMBOL_ITEM_FUNC(API_SystemHookDelete) 
    SYMBOL_ITEM_FUNC(_HookListInit) 
    SYMBOL_ITEM_FUNC(_SysInit) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolAddThread) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolCreate) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolDelete) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolDeleteThread) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolGetAttr) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolSetAttr) 
    SYMBOL_ITEM_FUNC(API_ThreadPoolStatus) 
    SYMBOL_ITEM_FUNC(_Allocate_ThreadPool_Object) 
    SYMBOL_ITEM_FUNC(_Free_ThreadPool_Object) 
    SYMBOL_ITEM_FUNC(_ThreadPoolInit) 
    SYMBOL_ITEM_FUNC(_bmsgCreate) 
    SYMBOL_ITEM_FUNC(_bmsgDelete) 
    SYMBOL_ITEM_FUNC(_bmsgFlush) 
    SYMBOL_ITEM_FUNC(_bmsgFreeByte) 
    SYMBOL_ITEM_FUNC(_bmsgGet) 
    SYMBOL_ITEM_FUNC(_bmsgIsEmpty) 
    SYMBOL_ITEM_FUNC(_bmsgIsFull) 
    SYMBOL_ITEM_FUNC(_bmsgNBytes) 
    SYMBOL_ITEM_FUNC(_bmsgNBytesNext) 
    SYMBOL_ITEM_FUNC(_bmsgPut) 
    SYMBOL_ITEM_FUNC(_bmsgSizeGet) 
    SYMBOL_ITEM_FUNC(_rngBufGet) 
    SYMBOL_ITEM_FUNC(_rngBufPut) 
    SYMBOL_ITEM_FUNC(_rngCreate) 
    SYMBOL_ITEM_FUNC(_rngDelete) 
    SYMBOL_ITEM_FUNC(_rngFlush) 
    SYMBOL_ITEM_FUNC(_rngFreeBytes) 
    SYMBOL_ITEM_FUNC(_rngIsEmpty) 
    SYMBOL_ITEM_FUNC(_rngIsFull) 
    SYMBOL_ITEM_FUNC(_rngMoveAhead) 
    SYMBOL_ITEM_FUNC(_rngNBytes) 
    SYMBOL_ITEM_FUNC(_rngPutAhead) 
    SYMBOL_ITEM_FUNC(_rngSizeGet) 
    SYMBOL_ITEM_FUNC(__aeabi_memclr) 
    SYMBOL_ITEM_FUNC(__aeabi_memclr4) 
    SYMBOL_ITEM_FUNC(__aeabi_memclr8) 
    SYMBOL_ITEM_FUNC(__aeabi_memcpy) 
    SYMBOL_ITEM_FUNC(__aeabi_memcpy4) 
    SYMBOL_ITEM_FUNC(__aeabi_memcpy8) 
    SYMBOL_ITEM_FUNC(__aeabi_memmove) 
    SYMBOL_ITEM_FUNC(__aeabi_memmove4) 
    SYMBOL_ITEM_FUNC(__aeabi_memmove8) 
    SYMBOL_ITEM_FUNC(__aeabi_memset) 
    SYMBOL_ITEM_FUNC(__aeabi_memset4) 
    SYMBOL_ITEM_FUNC(__aeabi_memset8) 
    SYMBOL_ITEM_FUNC(_ZdaPv) 
    SYMBOL_ITEM_FUNC(_ZdlPv) 
    SYMBOL_ITEM_FUNC(_Znaj) 
    SYMBOL_ITEM_FUNC(_Znwj) 
    SYMBOL_ITEM_FUNC(__cppRtDoCtors) 
    SYMBOL_ITEM_FUNC(__cppRtDoDtors) 
    SYMBOL_ITEM_FUNC(__cppRtDummy) 
    SYMBOL_ITEM_FUNC(__cxa_atexit) 
    SYMBOL_ITEM_FUNC(__cxa_finalize) 
    SYMBOL_ITEM_FUNC(__cxa_guard_abort) 
    SYMBOL_ITEM_FUNC(__cxa_guard_acquire) 
    SYMBOL_ITEM_FUNC(__cxa_guard_release) 
    SYMBOL_ITEM_FUNC(__cxa_module_finalize) 
    SYMBOL_ITEM_FUNC(__cxa_pure_virtual) 
    SYMBOL_ITEM_FUNC(_cppRtInit) 
    SYMBOL_ITEM_FUNC(_cppRtUninit) 
    SYMBOL_ITEM_FUNC(armAuxControlFeatureDisable) 
    SYMBOL_ITEM_FUNC(armAuxControlFeatureEnable) 
    SYMBOL_ITEM_FUNC(armControlFeatureDisable) 
    SYMBOL_ITEM_FUNC(armControlFeatureEnable) 
    SYMBOL_ITEM_FUNC(archIntHandle) 
    SYMBOL_ITEM_FUNC(bspCpuExcHook) 
    SYMBOL_ITEM_FUNC(archIntDisable) 
    SYMBOL_ITEM_FUNC(archIntEnable) 
    SYMBOL_ITEM_FUNC(archIntEnableForce) 
    SYMBOL_ITEM_FUNC(armVfp11HwInit) 
    SYMBOL_ITEM_FUNC(archMpCur) 
    SYMBOL_ITEM_FUNC(archMpInt) 
    SYMBOL_ITEM_FUNC(__diskCacheMemcpy) 
    SYMBOL_ITEM_FUNC(_Exit) 
    SYMBOL_ITEM_FUNC(lib_localeconv) 
    SYMBOL_ITEM_FUNC(lib_setlocale) 
    SYMBOL_ITEM_FUNC(init_etc_passwd_group) 
    SYMBOL_ITEM_FUNC(init_etc_shadow) 
    SYMBOL_ITEM_FUNC(_wcscasecmp) 
    SYMBOL_ITEM_FUNC(_wcsdup) 
    SYMBOL_ITEM_FUNC(_wcsncasecmp) 
    SYMBOL_ITEM_FUNC(lwip_platform_memcpy) 
    SYMBOL_ITEM_FUNC(lwip_platform_smemcpy) 
    SYMBOL_ITEM_FUNC(API_PciDevInterDisonnect) 
    SYMBOL_ITEM_FUNC(API_SysPerfInfo) 
    SYMBOL_ITEM_FUNC(API_SysPerfRefresh) 
    SYMBOL_ITEM_FUNC(API_SysPerfStart) 
    SYMBOL_ITEM_FUNC(API_SysPerfStop) 
    SYMBOL_ITEM_FUNC(__aeabi_atexit) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cas) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key) 
    SYMBOL_ITEM_OBJ(mbedtls_mutex_free) 
    SYMBOL_ITEM_OBJ(mbedtls_mutex_init) 
    SYMBOL_ITEM_OBJ(mbedtls_mutex_lock) 
    SYMBOL_ITEM_OBJ(mbedtls_mutex_unlock) 
    SYMBOL_ITEM_OBJ(mbedtls_threading_gmtime_mutex) 
    SYMBOL_ITEM_OBJ(mbedtls_threading_readdir_mutex) 
    SYMBOL_ITEM_OBJ(cdio_version_string) 
    SYMBOL_ITEM_OBJ(yaffs_trace_mask) 
    SYMBOL_ITEM_OBJ(yaffsfs_deviceList) 
    SYMBOL_ITEM_OBJ(_G_cacheopLib) 
    SYMBOL_ITEM_OBJ(_K_timingKernel) 
    SYMBOL_ITEM_OBJ(_K_ulNCpus) 
    SYMBOL_ITEM_OBJ(_K_pfuncTimespecTimeoutTick) 
    SYMBOL_ITEM_OBJ(_K_pfuncTimespecTimeoutTick64) 
    SYMBOL_ITEM_OBJ(_ctype_) 
    SYMBOL_ITEM_OBJ(_tolower_tab_) 
    SYMBOL_ITEM_OBJ(_toupper_tab_) 
    SYMBOL_ITEM_OBJ(tzname) 
    SYMBOL_ITEM_OBJ(reachable_time) 
    SYMBOL_ITEM_OBJ(retrans_timer) 
    SYMBOL_ITEM_OBJ(csma_mac_driver) 
    SYMBOL_ITEM_OBJ(null_mac_driver) 
    SYMBOL_ITEM_OBJ(null_rdc_driver) 
    SYMBOL_ITEM_OBJ(tdma_mac_driver) 
    SYMBOL_ITEM_OBJ(snmp_community) 
    SYMBOL_ITEM_OBJ(snmp_community_trap) 
    SYMBOL_ITEM_OBJ(snmp_community_write) 
    SYMBOL_ITEM_OBJ(mq_attr_default) 
    SYMBOL_ITEM_OBJ(_G_stShellStackSize) 
    SYMBOL_ITEM_OBJ(_G_symStatic) 
    SYMBOL_ITEM_OBJ(_G_foGMemDrv) 
    SYMBOL_ITEM_OBJ(_G_iPipeDrvNum) 
    SYMBOL_ITEM_OBJ(_G_ulPipeLockOpt) 
    SYMBOL_ITEM_OBJ(_G_iSpipeDrvNum) 
    SYMBOL_ITEM_OBJ(_G_ulSpipeLockOpt) 
    SYMBOL_ITEM_OBJ(_G_iTycoDrvNum) 
    SYMBOL_ITEM_OBJ(_G_ulMutexOptionsTyLib) 
    SYMBOL_ITEM_OBJ(console_printk) 
    SYMBOL_ITEM_OBJ(mbedtls_timing_alarmed) 
    SYMBOL_ITEM_OBJ(_G_plineDiskCacheHeader) 
    SYMBOL_ITEM_OBJ(_G_ulDiskCacheListLock) 
    SYMBOL_ITEM_OBJ(_K_ulInterShowLock) 
    SYMBOL_ITEM_OBJ(_K_plineVmmVAddrSpaceHeader) 
    SYMBOL_ITEM_OBJ(_G_plineVProcHeader) 
    SYMBOL_ITEM_OBJ(_G_ulVProcMutex) 
    SYMBOL_ITEM_OBJ(nd6_queue_size) 
    SYMBOL_ITEM_OBJ(aodv_addr_any) 
    SYMBOL_ITEM_OBJ(_null_auth) 
    SYMBOL_ITEM_OBJ(_G_uiNamedNodeCounter) 
    SYMBOL_ITEM_OBJ(_G_ulPosixLock) 
    SYMBOL_ITEM_OBJ(_G_hTShellLock) 
    SYMBOL_ITEM_OBJ(_G_pfuncTSVarHook) 
    SYMBOL_ITEM_OBJ(_G_stSymbolCounter) 
    SYMBOL_ITEM_OBJ(_G_stSymbolNameTotalLen) 
    SYMBOL_ITEM_OBJ(_G_ulSymbolTableLock) 
    SYMBOL_ITEM_OBJ(_G_hPciCtrlHandle) 
    SYMBOL_ITEM_OBJ(_G_iPciVerbose) 
    SYMBOL_ITEM_OBJ(_G_bPowerSavingMode) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_crt_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_key_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_ca_pwd_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cas_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cas_pem) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cas_pem_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_crt_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_cli_key_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_crt_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key_ec) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key_ec_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key_len) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key_rsa) 
    SYMBOL_ITEM_OBJ(mbedtls_test_srv_key_rsa_len) 
    SYMBOL_ITEM_OBJ(mbedtls_cipher_definitions) 
    SYMBOL_ITEM_OBJ(mbedtls_md2_info) 
    SYMBOL_ITEM_OBJ(mbedtls_md4_info) 
    SYMBOL_ITEM_OBJ(mbedtls_md5_info) 
    SYMBOL_ITEM_OBJ(mbedtls_ripemd160_info) 
    SYMBOL_ITEM_OBJ(mbedtls_sha1_info) 
    SYMBOL_ITEM_OBJ(mbedtls_sha224_info) 
    SYMBOL_ITEM_OBJ(mbedtls_sha256_info) 
    SYMBOL_ITEM_OBJ(mbedtls_sha384_info) 
    SYMBOL_ITEM_OBJ(mbedtls_sha512_info) 
    SYMBOL_ITEM_OBJ(mbedtls_ecdsa_info) 
    SYMBOL_ITEM_OBJ(mbedtls_eckey_info) 
    SYMBOL_ITEM_OBJ(mbedtls_eckeydh_info) 
    SYMBOL_ITEM_OBJ(mbedtls_rsa_alt_info) 
    SYMBOL_ITEM_OBJ(mbedtls_rsa_info) 
    SYMBOL_ITEM_OBJ(mbedtls_x509_crt_profile_default) 
    SYMBOL_ITEM_OBJ(mbedtls_x509_crt_profile_next) 
    SYMBOL_ITEM_OBJ(mbedtls_x509_crt_profile_suiteb) 
    SYMBOL_ITEM_OBJ(deflate_copyright) 
    SYMBOL_ITEM_OBJ(dictionary) 
    SYMBOL_ITEM_OBJ(hello) 
    SYMBOL_ITEM_OBJ(inflate_copyright) 
    SYMBOL_ITEM_OBJ(_dist_code) 
    SYMBOL_ITEM_OBJ(_length_code) 
    SYMBOL_ITEM_OBJ(z_errmsg) 
    SYMBOL_ITEM_OBJ(CDIO_SECTOR_SYNC_HEADER) 
    SYMBOL_ITEM_OBJ(libcdio_version_num) 
    SYMBOL_ITEM_OBJ(ISO_XA_MARKER_STRING) 
    SYMBOL_ITEM_OBJ(ISO_STANDARD_ID) 
    SYMBOL_ITEM_OBJ(nand_flash_ids) 
    SYMBOL_ITEM_OBJ(nand_manuf_ids) 
    SYMBOL_ITEM_OBJ(_K_cKernelLogo) 
    SYMBOL_ITEM_OBJ(_C_ctype_) 
    SYMBOL_ITEM_OBJ(_C_tolower_) 
    SYMBOL_ITEM_OBJ(_C_toupper_) 
    SYMBOL_ITEM_OBJ(_G_cMonth) 
    SYMBOL_ITEM_OBJ(_G_cWeek) 
    SYMBOL_ITEM_OBJ(in6addr_linklocal_allnodes) 
    SYMBOL_ITEM_OBJ(in6addr_loopback) 
    SYMBOL_ITEM_OBJ(in6addr_nodelocal_allnodes) 
    SYMBOL_ITEM_OBJ(ncrc_rev_table) 
    SYMBOL_ITEM_OBJ(crc16_table) 
    SYMBOL_ITEM_OBJ(netconn_aborted) 
    SYMBOL_ITEM_OBJ(netconn_closed) 
    SYMBOL_ITEM_OBJ(netconn_deleted) 
    SYMBOL_ITEM_OBJ(netconn_reset) 
    SYMBOL_ITEM_OBJ(dns_mquery_v4group) 
    SYMBOL_ITEM_OBJ(dns_mquery_v6group) 
    SYMBOL_ITEM_OBJ(ip_addr_any_type) 
    SYMBOL_ITEM_OBJ(memp_ARP_QUEUE) 
    SYMBOL_ITEM_OBJ(memp_FRAG_PBUF) 
    SYMBOL_ITEM_OBJ(memp_IGMP_GROUP) 
    SYMBOL_ITEM_OBJ(memp_IP6_REASSDATA) 
    SYMBOL_ITEM_OBJ(memp_LOCALHOSTLIST) 
    SYMBOL_ITEM_OBJ(memp_MLD6_GROUP) 
    SYMBOL_ITEM_OBJ(memp_ND6_QUEUE) 
    SYMBOL_ITEM_OBJ(memp_NETBUF) 
    SYMBOL_ITEM_OBJ(memp_NETCONN) 
    SYMBOL_ITEM_OBJ(memp_NETDB) 
    SYMBOL_ITEM_OBJ(memp_PBUF) 
    SYMBOL_ITEM_OBJ(memp_PBUF_POOL) 
    SYMBOL_ITEM_OBJ(memp_pools) 
    SYMBOL_ITEM_OBJ(memp_RAW_PCB) 
    SYMBOL_ITEM_OBJ(memp_REASSDATA) 
    SYMBOL_ITEM_OBJ(memp_SYS_TIMEOUT) 
    SYMBOL_ITEM_OBJ(memp_TCP_PCB) 
    SYMBOL_ITEM_OBJ(memp_TCP_PCB_LISTEN) 
    SYMBOL_ITEM_OBJ(memp_TCP_SEG) 
    SYMBOL_ITEM_OBJ(memp_TCPIP_MSG_API) 
    SYMBOL_ITEM_OBJ(memp_TCPIP_MSG_INPKT) 
    SYMBOL_ITEM_OBJ(memp_UDP_PCB) 
    SYMBOL_ITEM_OBJ(tcp_pcb_lists) 
    SYMBOL_ITEM_OBJ(lwip_cyclic_timers) 
    SYMBOL_ITEM_OBJ(lwip_num_cyclic_timers) 
    SYMBOL_ITEM_OBJ(ip_addr_any) 
    SYMBOL_ITEM_OBJ(ip_addr_broadcast) 
    SYMBOL_ITEM_OBJ(dhcp6_All_DHCP6_Relay_Agents_and_Servers) 
    SYMBOL_ITEM_OBJ(dhcp6_All_DHCP6_Servers) 
    SYMBOL_ITEM_OBJ(in6addr_any) 
    SYMBOL_ITEM_OBJ(ip6_addr_any) 
    SYMBOL_ITEM_OBJ(ethbroadcast) 
    SYMBOL_ITEM_OBJ(ethzero) 
    SYMBOL_ITEM_OBJ(md5_digest) 
    SYMBOL_ITEM_OBJ(chap_protent) 
    SYMBOL_ITEM_OBJ(chapms_digest) 
    SYMBOL_ITEM_OBJ(chapms2_digest) 
    SYMBOL_ITEM_OBJ(eap_protent) 
    SYMBOL_ITEM_OBJ(ipcp_protent) 
    SYMBOL_ITEM_OBJ(ipv6cp_protent) 
    SYMBOL_ITEM_OBJ(lcp_protent) 
    SYMBOL_ITEM_OBJ(memp_PPP_PCB) 
    SYMBOL_ITEM_OBJ(protocols) 
    SYMBOL_ITEM_OBJ(memp_PPPOE_IF) 
    SYMBOL_ITEM_OBJ(memp_PPPOL2TP_PCB) 
    SYMBOL_ITEM_OBJ(memp_PPPOS_PCB) 
    SYMBOL_ITEM_OBJ(pap_protent) 
    SYMBOL_ITEM_OBJ(snmp_zero_dot_zero) 
    SYMBOL_ITEM_OBJ(snmp_zero_dot_zero_values) 
    SYMBOL_ITEM_OBJ(mib2) 
    SYMBOL_ITEM_OBJ(snmp_mib2_icmp_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_interface_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_at_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_ip_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_snmp_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_system_node) 
    SYMBOL_ITEM_OBJ(snmp_mib2_tcp_root) 
    SYMBOL_ITEM_OBJ(snmp_mib2_udp_root) 
    SYMBOL_ITEM_OBJ(snmpframeworkmib) 
    SYMBOL_ITEM_OBJ(usmAESPrivProtocol) 
    SYMBOL_ITEM_OBJ(usmDESPrivProtocol) 
    SYMBOL_ITEM_OBJ(usmHMACMD5AuthProtocol) 
    SYMBOL_ITEM_OBJ(usmHMACSHAAuthProtocol) 
    SYMBOL_ITEM_OBJ(usmNoAuthProtocol) 
    SYMBOL_ITEM_OBJ(usmNoPrivProtocol) 
    SYMBOL_ITEM_OBJ(snmpusmmib) 
    SYMBOL_ITEM_OBJ(ptr_to_globals) 
    SYMBOL_ITEM_OBJ(mbedtls_cipher_supported) 
    SYMBOL_ITEM_OBJ(dictId) 
    SYMBOL_ITEM_OBJ(prog) 
    SYMBOL_ITEM_OBJ(_G_openocdInfo) 
    SYMBOL_ITEM_OBJ(iso_rock_enums) 
    SYMBOL_ITEM_OBJ(iso_rock_nm_flag) 
    SYMBOL_ITEM_OBJ(iso_rock_sl_flag) 
    SYMBOL_ITEM_OBJ(iso_rock_tf_flag) 
    SYMBOL_ITEM_OBJ(debugger_xa_misc_enum) 
    SYMBOL_ITEM_OBJ(iso_enums1) 
    SYMBOL_ITEM_OBJ(iso_extension_enums) 
    SYMBOL_ITEM_OBJ(iso_flag_enums) 
    SYMBOL_ITEM_OBJ(iso_vd_enums) 
    SYMBOL_ITEM_OBJ(mtd_table) 
    SYMBOL_ITEM_OBJ(_G_pfsrRoot) 
    SYMBOL_ITEM_OBJ(_G_ulProcFsLock) 
    SYMBOL_ITEM_OBJ(_G_devhdrRoot) 
    SYMBOL_ITEM_OBJ(_G_rfsrRoot) 
    SYMBOL_ITEM_OBJ(yaffs_wr_attempts) 
    SYMBOL_ITEM_OBJ(_K__partBuffer) 
    SYMBOL_ITEM_OBJ(_K_atomic64KernelTime) 
    SYMBOL_ITEM_OBJ(_K_bHeapCrossBorderEn) 
    SYMBOL_ITEM_OBJ(_K_cpuTable) 
    SYMBOL_ITEM_OBJ(_K_esBuffer) 
    SYMBOL_ITEM_OBJ(_K_eventBuffer) 
    SYMBOL_ITEM_OBJ(_K_heapBuffer) 
    SYMBOL_ITEM_OBJ(_K_hookKernel) 
    SYMBOL_ITEM_OBJ(_K_idescTable) 
    SYMBOL_ITEM_OBJ(_K_iTODDelta) 
    SYMBOL_ITEM_OBJ(_K_iTODDeltaNs) 
    SYMBOL_ITEM_OBJ(_K_jobmsgKernel) 
    SYMBOL_ITEM_OBJ(_K_jobqKernel) 
    SYMBOL_ITEM_OBJ(_K_klKernel) 
    SYMBOL_ITEM_OBJ(_K_msgqueueBuffer) 
    SYMBOL_ITEM_OBJ(_K_pcbbmapGlobalReady) 
    SYMBOL_ITEM_OBJ(_K_pfuncKernelDebugError) 
    SYMBOL_ITEM_OBJ(_K_pfuncKernelDebugLog) 
    SYMBOL_ITEM_OBJ(_K_pheapKernel) 
    SYMBOL_ITEM_OBJ(_K_pheapSystem) 
    SYMBOL_ITEM_OBJ(_K_plineTCBHeader) 
    SYMBOL_ITEM_OBJ(_K_ptcbIdle) 
    SYMBOL_ITEM_OBJ(_K_ptcbTCBIdTable) 
    SYMBOL_ITEM_OBJ(_K_resrcEvent) 
    SYMBOL_ITEM_OBJ(_K_resrcEventSet) 
    SYMBOL_ITEM_OBJ(_K_resrcHeap) 
    SYMBOL_ITEM_OBJ(_K_resrcMsgQueue) 
    SYMBOL_ITEM_OBJ(_K_resrcPart) 
    SYMBOL_ITEM_OBJ(_K_resrcRms) 
    SYMBOL_ITEM_OBJ(_K_resrcTcb) 
    SYMBOL_ITEM_OBJ(_K_resrcTmr) 
    SYMBOL_ITEM_OBJ(_K_rmsBuffer) 
    SYMBOL_ITEM_OBJ(_K_slcaAtomic) 
    SYMBOL_ITEM_OBJ(_K_slcaVectorTable) 
    SYMBOL_ITEM_OBJ(_K_slKernTime) 
    SYMBOL_ITEM_OBJ(_K_stkFreeFlag) 
    SYMBOL_ITEM_OBJ(_K_stkInterruptStack) 
    SYMBOL_ITEM_OBJ(_K_tcbBuffer) 
    SYMBOL_ITEM_OBJ(_K_tcbDummy) 
    SYMBOL_ITEM_OBJ(_K_tmrBuffer) 
    SYMBOL_ITEM_OBJ(_K_tvTODCurrent) 
    SYMBOL_ITEM_OBJ(_K_tvTODMono) 
    SYMBOL_ITEM_OBJ(_K_twjTable) 
    SYMBOL_ITEM_OBJ(_K_ucSysStatus) 
    SYMBOL_ITEM_OBJ(_K_ulCPUUsageEnable) 
    SYMBOL_ITEM_OBJ(_K_ulCPUUsageKernelTicks) 
    SYMBOL_ITEM_OBJ(_K_ulCPUUsageTicks) 
    SYMBOL_ITEM_OBJ(_K_ulIdleId) 
    SYMBOL_ITEM_OBJ(_K_ulKernFlags) 
    SYMBOL_ITEM_OBJ(_K_ulNotRunError) 
    SYMBOL_ITEM_OBJ(_K_ulThreadItimerId) 
    SYMBOL_ITEM_OBJ(_K_usThreadCounter) 
    SYMBOL_ITEM_OBJ(_K_wuDelay) 
    SYMBOL_ITEM_OBJ(_K_wuHTmr) 
    SYMBOL_ITEM_OBJ(_K_wuITmr) 
    SYMBOL_ITEM_OBJ(_K_wuWatchDog) 
    SYMBOL_ITEM_OBJ(_K_pfuncHeapTraceAlloc) 
    SYMBOL_ITEM_OBJ(_K_pfuncHeapTraceFree) 
    SYMBOL_ITEM_OBJ(_G_vmzonePhysical) 
    SYMBOL_ITEM_OBJ(_G_mmuOpLib) 
    SYMBOL_ITEM_OBJ(_G_ulVmmLock) 
    SYMBOL_ITEM_OBJ(__sdidinit) 
    SYMBOL_ITEM_OBJ(__sglue) 
    SYMBOL_ITEM_OBJ(timezone) 
    SYMBOL_ITEM_OBJ(_G_ulExecShareLock) 
    SYMBOL_ITEM_OBJ(_G_pvprocTable) 
    SYMBOL_ITEM_OBJ(_G_vprocKernel) 
    SYMBOL_ITEM_OBJ(_G_dpmaBuffer) 
    SYMBOL_ITEM_OBJ(_G_resrcDpma) 
    SYMBOL_ITEM_OBJ(_proto_stayopen) 
    SYMBOL_ITEM_OBJ(_serv_stayopen) 
    SYMBOL_ITEM_OBJ(_G_ulNetLibcLock) 
    SYMBOL_ITEM_OBJ(h_errno) 
    SYMBOL_ITEM_OBJ(lock_tcpip_core) 
    SYMBOL_ITEM_OBJ(ip_data) 
    SYMBOL_ITEM_OBJ(memp_memory_ARP_QUEUE_base) 
    SYMBOL_ITEM_OBJ(memp_memory_FRAG_PBUF_base) 
    SYMBOL_ITEM_OBJ(memp_memory_IGMP_GROUP_base) 
    SYMBOL_ITEM_OBJ(memp_memory_IP6_REASSDATA_base) 
    SYMBOL_ITEM_OBJ(memp_memory_LOCALHOSTLIST_base) 
    SYMBOL_ITEM_OBJ(memp_memory_MLD6_GROUP_base) 
    SYMBOL_ITEM_OBJ(memp_memory_ND6_QUEUE_base) 
    SYMBOL_ITEM_OBJ(memp_memory_NETBUF_base) 
    SYMBOL_ITEM_OBJ(memp_memory_NETCONN_base) 
    SYMBOL_ITEM_OBJ(memp_memory_NETDB_base) 
    SYMBOL_ITEM_OBJ(memp_memory_PBUF_base) 
    SYMBOL_ITEM_OBJ(memp_memory_PBUF_POOL_base) 
    SYMBOL_ITEM_OBJ(memp_memory_RAW_PCB_base) 
    SYMBOL_ITEM_OBJ(memp_memory_REASSDATA_base) 
    SYMBOL_ITEM_OBJ(memp_memory_SYS_TIMEOUT_base) 
    SYMBOL_ITEM_OBJ(memp_memory_TCP_PCB_base) 
    SYMBOL_ITEM_OBJ(memp_memory_TCP_PCB_LISTEN_base) 
    SYMBOL_ITEM_OBJ(memp_memory_TCP_SEG_base) 
    SYMBOL_ITEM_OBJ(memp_memory_TCPIP_MSG_API_base) 
    SYMBOL_ITEM_OBJ(memp_memory_TCPIP_MSG_INPKT_base) 
    SYMBOL_ITEM_OBJ(memp_memory_UDP_PCB_base) 
    SYMBOL_ITEM_OBJ(netif_default) 
    SYMBOL_ITEM_OBJ(netif_list) 
    SYMBOL_ITEM_OBJ(pbuf_free_ooseq_pending) 
    SYMBOL_ITEM_OBJ(raw_pcbs) 
    SYMBOL_ITEM_OBJ(lwip_stats) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs_changed) 
    SYMBOL_ITEM_OBJ(tcp_bound_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_listen_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_ticks) 
    SYMBOL_ITEM_OBJ(tcp_tw_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_input_pcb) 
    SYMBOL_ITEM_OBJ(udp_pcbs) 
    SYMBOL_ITEM_OBJ(dhcp_rx_options_given) 
    SYMBOL_ITEM_OBJ(dhcp_rx_options_val) 
    SYMBOL_ITEM_OBJ(dhcp6_rx_options) 
    SYMBOL_ITEM_OBJ(default_router_list) 
    SYMBOL_ITEM_OBJ(destination_cache) 
    SYMBOL_ITEM_OBJ(neighbor_cache) 
    SYMBOL_ITEM_OBJ(prefix_list) 
    SYMBOL_ITEM_OBJ(aodv_ipaddr_any) 
    SYMBOL_ITEM_OBJ(aodv_netif) 
    SYMBOL_ITEM_OBJ(aodv_host_state) 
    SYMBOL_ITEM_OBJ(aodv_rt_tbl) 
    SYMBOL_ITEM_OBJ(memp_memory_PPP_PCB_base) 
    SYMBOL_ITEM_OBJ(memp_memory_PPPOE_IF_base) 
    SYMBOL_ITEM_OBJ(memp_memory_PPPOL2TP_PCB_base) 
    SYMBOL_ITEM_OBJ(memp_memory_PPPOS_PCB_base) 
    SYMBOL_ITEM_OBJ(lowpan_addr_context_t) 
    SYMBOL_ITEM_OBJ(netbiosns_pcb) 
    SYMBOL_ITEM_OBJ(__key_decryptsession_pk_LOCAL) 
    SYMBOL_ITEM_OBJ(__key_encryptsession_pk_LOCAL) 
    SYMBOL_ITEM_OBJ(__key_gendes_LOCAL) 
    SYMBOL_ITEM_OBJ(rpc_createerr) 
    SYMBOL_ITEM_OBJ(svc_fdset) 
    SYMBOL_ITEM_OBJ(svc_max_pollfd) 
    SYMBOL_ITEM_OBJ(svc_pollfd) 
    SYMBOL_ITEM_OBJ(snmp_stats) 
    SYMBOL_ITEM_OBJ(snmp_mib2_lwip_locks) 
    SYMBOL_ITEM_OBJ(snmp_inform_callback) 
    SYMBOL_ITEM_OBJ(snmp_inform_callback_arg) 
    SYMBOL_ITEM_OBJ(snmp_write_callback) 
    SYMBOL_ITEM_OBJ(snmp_write_callback_arg) 
    SYMBOL_ITEM_OBJ(username) 
    SYMBOL_ITEM_OBJ(snmp_traps_handle) 
    SYMBOL_ITEM_OBJ(_G_aioqueue) 
    SYMBOL_ITEM_OBJ(_G_plinePxNameNodeHash) 
    SYMBOL_ITEM_OBJ(_G_plineSymbolHeaderHashTbl) 
    SYMBOL_ITEM_OBJ(_G_ulSpipeReadOpenLock) 
    SYMBOL_ITEM_OBJ(_G_ulSpipeWriteOpenLock) 
    SYMBOL_ITEM_OBJ(_S_atomicFileLineOp) 
    SYMBOL_ITEM_OBJ(_S_bFileEntryRemoveReq) 
    SYMBOL_ITEM_OBJ(_S_deventryTbl) 
    SYMBOL_ITEM_OBJ(_S_iIoMaxLinkLevels) 
    SYMBOL_ITEM_OBJ(_S_ioeIoGlobalEnv) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileDescGet) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileDup) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileDup2) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileGet) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileRefDec) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileRefGet) 
    SYMBOL_ITEM_OBJ(_S_pfuncFileRefInc) 
    SYMBOL_ITEM_OBJ(_S_pfuncGetCurPid) 
    SYMBOL_ITEM_OBJ(_S_plineDevHdrHeader) 
    SYMBOL_ITEM_OBJ(_S_plineFileEntryHeader) 
    SYMBOL_ITEM_OBJ(_S_resrcThreadPool) 
    SYMBOL_ITEM_OBJ(_S_threadpoolBuffer) 
    SYMBOL_ITEM_OBJ(_S_ulPowerMId) 
    SYMBOL_ITEM_OBJ(_S_ulThreadExceId) 
    SYMBOL_ITEM_OBJ(_S_ulThreadLogId) 
    SYMBOL_ITEM_OBJ(_G_hLogMsgHandle) 
    SYMBOL_ITEM_OBJ(_G_plinePMAdapter) 
    SYMBOL_ITEM_OBJ(_G_plinePMDev) 
    SYMBOL_ITEM_OBJ(_G_ulPowerMLock) 
    SYMBOL_ITEM_OBJ(_G_hookTable) 
    SYMBOL_ITEM_OBJ(__IO_BASE__) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
SYMBOL_TABLE_END										
#endif													
/*********************************************************************************************************	
  END													
*********************************************************************************************************/	
