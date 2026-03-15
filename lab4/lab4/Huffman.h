#pragma once

#include "../../../OOP_Lab/lab3/lab3/BoolVector.h"
#include <string>
#include <unordered_map>
#include <memory>

class Huffman
{
public:
	struct Node {
		unsigned char symbol;
		size_t frequency;
		Node* left;
		Node* right;

		Node(unsigned char sym, size_t freq) 
			: symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}
		Node(Node* l, Node* r) 
			: symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
		~Node() {
			delete left;
			delete right;
		}
	};

public:
	Huffman() : root_(nullptr) {}
	~Huffman();

	// Build Huffman tree from file
	void build(const std::string& filename);

	// Encode file
	double encode(const std::string& input_file, const std::string& output_file);

	// Decode file
	bool decode(const std::string& input_file, const std::string& output_file);

	// Export tree to file
	bool exportTree(const std::string& filename) const;

	// Import tree from file
	bool importTree(const std::string& filename);

private:
	Node* root_;
	std::unordered_map<unsigned char, BoolVector> codes_;

	// Helper methods
	void buildCodes(Node* node, BoolVector& current_code);
	void deleteTree();
	void encodeSymbol(unsigned char symbol, std::string& bits) const;
	void serializeTree(Node* node, std::string& data) const;
	Node* deserializeTree(const std::string& data, size_t& pos);
};