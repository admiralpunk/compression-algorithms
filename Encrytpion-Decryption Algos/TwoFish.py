from Crypto.Cipher import Twofish
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes

key = get_random_bytes(16)  # 128-bit key
cipher = Twofish.new(key, Twofish.MODE_CBC)

data = b"Sensitive data to encrypt"
ciphertext = cipher.encrypt(pad(data, Twofish.block_size))

# Decryption
cipher_decrypt = Twofish.new(key, Twofish.MODE_CBC, cipher.iv)
decrypted_data = unpad(cipher_decrypt.decrypt(ciphertext), Twofish.block_size)
print(decrypted_data.decode('utf-8'))
