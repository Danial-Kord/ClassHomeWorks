from cryptography.fernet import Fernet




def encrypt_message(message, key):

    encoded_message = message.encode()
    f = Fernet(key)
    encrypted_message = f.encrypt(encoded_message)

    print("encryption : ")
    print(encrypted_message)
    return encrypted_message

def decrypt_message(message, key):
    f = Fernet(key)
    decrypted_message = f.decrypt(message)
    print("decryption : " + decrypted_message.decode())

if __name__ == "__main__":
    key = Fernet.generate_key()
    str = encrypt_message(input("enter your inout :"),key)
    decrypt_message(str, key)