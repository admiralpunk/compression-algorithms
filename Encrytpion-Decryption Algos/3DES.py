from Crypto.Cipher import DES3
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes

key = get_random_bytes(24)  # 3DES key size
cipher = DES3.new(key, DES3.MODE_CBC)

data = b"Sensitive data to encrypt"
ciphertext = cipher.encrypt(pad(data, DES3.block_size))

# Decryption
cipher_decrypt = DES3.new(key, DES3.MODE_CBC, cipher.iv)
decrypted_data = unpad(cipher_decrypt.decrypt(ciphertext), DES3.block_size)
print(decrypted_data.decode('utf-8'))
