def lzw_encoding(data):
    # Create dictionary of initial characters
    dictionary = {chr(i): i for i in range(256)}
    code = 256
    w = ""
    encoded_data = []
    
    for char in data:
        wc = w + char
        if wc in dictionary:
            w = wc
        else:
            encoded_data.append(dictionary[w])
            dictionary[wc] = code
            code += 1
            w = char
    
    if w:
        encoded_data.append(dictionary[w])
    return encoded_data

def lzw_decoding(encoded_data):
    dictionary = {i: chr(i) for i in range(256)}
    code = 256
    w = chr(encoded_data.pop(0))
    decoded_data = w
    for k in encoded_data:
        if k in dictionary:
            entry = dictionary[k]
        elif k == code:
            entry = w + w[0]
        decoded_data += entry
        dictionary[code] = w + entry[0]
        code += 1
        w = entry
    return decoded_data

# Example usage:
data = "TOBEORNOTTOBEORTOBEORNOT"
encoded_data = lzw_encoding(data)
decoded_data = lzw_decoding(encoded_data)

print(f"Original: {data}")
print(f"Encoded: {encoded_data}")
print(f"Decoded: {decoded_data}")
