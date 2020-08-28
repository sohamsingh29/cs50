from sys import argv
from crypt import crypt
def main():
    t = argv[1]
    salt = f"{t[0]}{t[1]}"
    decipher(t,salt)
def decipher(t,salt):
    password = ""
    alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    for i in alphabet:  #for 1 length passwords
        password = f"{i}"
        hashed = crypt(password,salt)
        if (hashed == t):
            print(password)
        for j in alphabet:  #for 2 length passwords
            password = f"{i}{j}"
            hashed = crypt(password,salt)
            if (hashed == t):
                print(password)
            for k in alphabet:  #for 3 length passwords
                password = f"{i}{j}{k}"
                hashed = crypt(password,salt)
                if (hashed == t):
                    print(password)
                for l in alphabet:  #for 4 length passwords
                    password = f"{i}{j}{k}{l}"
                    hashed = crypt(password,salt)
                    if (hashed == t):
                        print(password)
                    for m in alphabet:  #for five length passwords
                        password = f"{i}{j}{k}{l}{m}"
                        hashed = crypt(password,salt)
                        if (hashed == t):
                            print(password)
#to call main as main function
if __name__=='__main__':
    main()