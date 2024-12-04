import heapq
from collections import defaultdict

# Build frequency dictionary
def build_freq_dict(data):
    freq = defaultdict(int)
    for char in data:
        freq[char] += 1
    return freq

# Build the Huffman Tree
def build_huffman_tree(freq_dict):
    heap = [[weight, [char, ""]] for char, weight in freq_dict.items()]
    heapq.heapify(heap)
    
    while len(heap) > 1:
        lo = heapq.heappop(heap)
        hi = heapq.heappop(heap)
        for pair in lo[1:]:
            pair[1] = '0' + pair[1]
        for pair in hi[1:]:
            pair[1] = '1' + pair[1]
        heapq.heappush(heap, [lo[0] + hi[0]] + lo[1:] + hi[1:])
    
    return heap[0]

# Encode the data using Huffman Tree
def huffman_encoding(data):
    freq_dict = build_freq_dict(data)
    huffman_tree = build_huffman_tree(freq_dict)
    huff_code = {char: code for char, code in huffman_tree[1:]}
    
    encoded_data = ''.join(huff_code[char] for char in data)
    return encoded_data, huff_code

# Decode the encoded data
def huffman_decoding(encoded_data, huff_code):
    reversed_code = {v: k for k, v in huff_code.items()}
    current_code = ''
    decoded_data = ''
    
    for bit in encoded_data:
        current_code += bit
        if current_code in reversed_code:
            decoded_data += reversed_code[current_code]
            current_code = ''
    
    return decoded_data

# Example usage:
data = "this is an example for huffman encoding"
encoded_data, huff_code = huffman_encoding(data)
decoded_data = huffman_decoding(encoded_data, huff_code)

print(f"Original: {data}")
print(f"Encoded: {encoded_data}")
print(f"Decoded: {decoded_data}")
