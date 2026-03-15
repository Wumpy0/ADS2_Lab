#include <iostream>
#include <fstream>
#include <queue>
#include <cctype>
#include "Huffman.h"

// Comparator for priority queue
struct CompareNode {
    bool operator()(Huffman::Node* left, Huffman::Node* right) {
        return left->frequency > right->frequency;
    }
};

Huffman::~Huffman() {
    deleteTree();
}

void Huffman::deleteTree() {
    delete root_;
    root_ = nullptr;
    codes_.clear();
}

void Huffman::build(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // Clear previous tree
    deleteTree();

    // Count frequencies
    std::unordered_map<unsigned char, size_t> frequencies;
    unsigned char byte;
    while (file.get(reinterpret_cast<char&>(byte))) {
        frequencies[byte]++;
    }
    file.close();

    if (frequencies.empty()) {
        return;
    }

    // Build Huffman tree using priority queue
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    for (auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }

    root_ = pq.top();

    // Build code table
    BoolVector current_code;
    buildCodes(root_, current_code);
}

void Huffman::buildCodes(Node* node, BoolVector& current_code) {
    if (!node) return;

    // Leaf node
    if (!node->left && !node->right) {
        codes_[node->symbol] = current_code;
        return;
    }

    // Process left subtree (add 0)
    BoolVector left_code(current_code.length() + 1, false);
    for (size_t i = 0; i < current_code.length(); ++i) {
        if (current_code[i]) {
            left_code.set(i);
        }
    }
    buildCodes(node->left, left_code);

    // Process right subtree (add 1)
    BoolVector right_code(current_code.length() + 1, false);
    for (size_t i = 0; i < current_code.length(); ++i) {
        if (current_code[i]) {
            right_code.set(i);
        }
    }
    right_code.set(current_code.length());
    buildCodes(node->right, right_code);
}

double Huffman::encode(const std::string& input_file, const std::string& output_file) {
    if (!root_) {
        build(input_file);
    }

    if (!root_) {
        return -1.0;
    }

    std::ifstream infile(input_file, std::ios::binary);
    if (!infile.is_open()) {
        return -1.0;
    }

    // Read all data
    std::string data((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();

    if (data.empty()) {
        return -1.0;
    }

    size_t original_bits = data.size() * 8;
    size_t encoded_bits = 0;

    // Calculate total bits needed for encoded data
    for (unsigned char byte : data) {
        if (codes_.find(byte) != codes_.end()) {
            encoded_bits += codes_[byte].length();
        }
    }

    // Create BoolVector for encoded data
    BoolVector encoded(encoded_bits, false);
    size_t bit_index = 0;

    // Encode data
    for (unsigned char byte : data) {
        if (codes_.find(byte) == codes_.end()) {
            return -1.0;
        }

        BoolVector& code = codes_[byte];
        for (size_t i = 0; i < code.length(); ++i) {
            if (code[i]) {
                encoded.set(bit_index);
            }
            bit_index++;
        }
    }

    // Write to output file
    std::ofstream outfile(output_file, std::ios::binary);
    if (!outfile.is_open()) {
        return -1.0;
    }

    // Write original size and bit count
    size_t original_size = data.size();
    outfile.write(reinterpret_cast<const char*>(&original_size), sizeof(original_size));
    outfile.write(reinterpret_cast<const char*>(&encoded_bits), sizeof(encoded_bits));

    // Write encoded data
    size_t encoded_bytes = (encoded_bits + 7) / 8;
    for (size_t i = 0; i < encoded_bytes; ++i) {
        unsigned char byte = 0;
        for (size_t j = 0; j < 8 && i * 8 + j < encoded_bits; ++j) {
            if (encoded[i * 8 + j]) {
                byte |= (1 << j);
            }
        }
        outfile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }

    outfile.close();

    // Calculate compression ratio
    double compression_ratio = static_cast<double>(original_bits) / encoded_bits;
    return compression_ratio;
}

bool Huffman::decode(const std::string& input_file, const std::string& output_file) {
    if (!root_) {
        return false;
    }

    std::ifstream infile(input_file, std::ios::binary);
    if (!infile.is_open()) {
        return false;
    }

    // Read metadata
    size_t original_size = 0;
    size_t encoded_bits = 0;

    if (!infile.read(reinterpret_cast<char*>(&original_size), sizeof(original_size))) {
        infile.close();
        return false;
    }

    if (!infile.read(reinterpret_cast<char*>(&encoded_bits), sizeof(encoded_bits))) {
        infile.close();
        return false;
    }

    // Read encoded data
    BoolVector encoded(encoded_bits, false);
    size_t encoded_bytes = (encoded_bits + 7) / 8;

    for (size_t i = 0; i < encoded_bytes; ++i) {
        unsigned char byte = 0;
        if (!infile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
            infile.close();
            return false;
        }

        for (size_t j = 0; j < 8 && i * 8 + j < encoded_bits; ++j) {
            if (byte & (1 << j)) {
                encoded.set(i * 8 + j);
            }
        }
    }

    infile.close();

    // Decode data
    std::string decoded_data;
    Node* current = root_;
    size_t decoded_count = 0;

    for (size_t i = 0; i < encoded_bits; ++i) {
        if (encoded[i]) {
            current = current->right;
        }
        else {
            current = current->left;
        }

        // Leaf node found
        if (!current->left && !current->right) {
            decoded_data += current->symbol;
            decoded_count++;
            current = root_;

            if (decoded_count >= original_size) {
                break;
            }
        }

        if (!current) {
            return false;
        }
    }

    // Write decoded data to file
    std::ofstream outfile(output_file, std::ios::binary);
    if (!outfile.is_open()) {
        return false;
    }

    outfile.write(decoded_data.c_str(), decoded_data.size());
    outfile.close();

    return decoded_data.size() == original_size;
}

bool Huffman::exportTree(const std::string& filename) const {
    if (!root_) {
        return false;
    }

    std::string tree_data;
    serializeTree(root_, tree_data);

    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        return false;
    }

    size_t size = tree_data.size();
    outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outfile.write(tree_data.c_str(), tree_data.size());
    outfile.close();

    return true;
}

bool Huffman::importTree(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        return false;
    }

    size_t size = 0;
    if (!infile.read(reinterpret_cast<char*>(&size), sizeof(size))) {
        infile.close();
        return false;
    }

    std::string tree_data(size, 0);
    if (!infile.read(&tree_data[0], size)) {
        infile.close();
        return false;
    }

    infile.close();

    deleteTree();
    size_t pos = 0;
    root_ = deserializeTree(tree_data, pos);

    // Rebuild codes
    BoolVector current_code;
    buildCodes(root_, current_code);

    return root_ != nullptr;
}

void Huffman::serializeTree(Node* node, std::string& data) const {
    if (!node) return;

    // Leaf node
    if (!node->left && !node->right) {
        data += '1';  // Mark as leaf
        data += node->symbol;
        return;
    }

    data += '0';  // Mark as internal node
    serializeTree(node->left, data);
    serializeTree(node->right, data);
}

Huffman::Node* Huffman::deserializeTree(const std::string& data, size_t& pos) {
    if (pos >= data.size()) {
        return nullptr;
    }

    if (data[pos] == '1') {
        // Leaf node
        pos++;
        if (pos >= data.size()) {
            return nullptr;
        }
        unsigned char symbol = data[pos];
        pos++;
        return new Node(symbol, 0);
    }
    else {
        // Internal node
        pos++;
        Node* left = deserializeTree(data, pos);
        Node* right = deserializeTree(data, pos);
        if (!left || !right) {
            delete left;
            delete right;
            return nullptr;
        }
        return new Node(left, right);
    }
}