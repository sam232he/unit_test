# How To Use
1- just include aes_cbc.h file and call below API :  
~~~
aes_perform_cbc(uint16_t *const plain_arr_p, uint16_t length, const uint16_t *const orig_key_p,		
		AES_OPERATION_MODE mode,uint16_t *const cipher_arr_p)	
~~~		
plain_arr_p : input array to be encrypted/decrypted.    
length : size of input array.    
orig_key_p : key array for encryption(must be 16 bytes since we use cbc128).    
mode : AES_OPMODE_ENCRYPT or AES_OPMODE_DECRYPT    
cipher_arr_p : output encrypted/decrypted data.    