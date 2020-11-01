import hashlib




str = "if you want to keep a secret, you must also hide it from yourself"

print("statement : " + str)

result = hashlib.sha256(str.encode())


print("Encryption for SHA256: ")
print(result.hexdigest())
print("Encryption for SHA256 binary: ")
s = result.hexdigest()
s = ''.join(format(ord(x), 'b') for x in s)
print(s)

print()

str2 = "if you wnt to keep a secret, you must also hide it from yourself"

print("statement2 : " + str2)

result = hashlib.sha256(str2.encode())


print("Encryption for SHA256: ")
print(result.hexdigest())
print("Encryption for SHA256 binary: ")
s2 = result.hexdigest()
s2 = ''.join(format(ord(x), 'b') for x in s2)
print(s2)

print()

print("difference between 2 SHA256 encrypted statements in bits : ")
out= s.__len__() - s2.__len__()
for i in range(0,s2.__len__()):
    if s[i] != s2[i]:
        out += 1

print(out)



