from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes

key = get_random_bytes(32)  # 256-bit key
cipher = AES.new(key, AES.MODE_CBC)

data = b"Sensitive data to encrypt"
ciphertext = cipher.encrypt(pad(data, AES.block_size))

# Decryption
cipher_decrypt = AES.new(key, AES.MODE_CBC, cipher.iv)
decrypted_data = unpad(cipher_decrypt.decrypt(ciphertext), AES.block_size)
print(decrypted_data.decode('utf-8'))
