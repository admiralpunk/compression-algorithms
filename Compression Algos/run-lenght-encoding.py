def run_length_encoding(data):
    encoding = ""
    prev_char = ""
    count = 1
    
    for char in data:
        if char == prev_char:
            count += 1
        else:
            if prev_char:
                encoding += str(count) + prev_char
            count = 1
            prev_char = char
    encoding += str(count) + prev_char
    return encoding

def run_length_decoding(encoded_data):
    decoded_data = ""
    i = 0
    while i < len(encoded_data):
        count = int(encoded_data[i])
        char = encoded_data[i + 1]
        decoded_data += char * count
        i += 2
    return decoded_data

# Example usage:
data = "AAAABBBCCDAA"
encoded_data = run_length_encoding(data)
decoded_data = run_length_decoding(encoded_data)

print(f"Original: {data}")
print(f"Encoded: {encoded_data}")
print(f"Decoded: {decoded_data}")
