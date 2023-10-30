import hashlib
import os
import base64
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.fernet import Fernet

def decrypt_word(encrypted_word, seed_phrase):
    password = hashlib.sha256(seed_phrase.encode()).hexdigest().encode()
    salt = b'salt_'
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256,
        iterations=100000,
        salt=salt,
        length=32,
        backend=default_backend()
    )
    key = base64.urlsafe_b64encode(kdf.derive(password))
    f = Fernet(key)
    decrypted_word = f.decrypt(encrypted_word).decode()

    return decrypted_word

# Example usage
encrypted_word = b'3a118ca3ae1a8984bf2588ac2f42fb68293847c49b116a66e5e8d4387f58cf48'
seed_phrase = ''
decrypted_word = decrypt_word(encrypted_word, seed_phrase)
print(decrypted_word)
