//
// Created by dmitry on 4/2/20.
//

#include "Json.h"

using namespace std;

namespace Json {

Document::Document(Node root) : root(move(root)) {}

const Node &Document::GetRoot() const { return root; }

Node LoadNode(istream &input);

Node LoadArray(istream &input) {
  vector<Node> result;

  for (char c; input >> c && c != ']';) {
    if (c != ',') {
      input.putback(c);
    }
    result.push_back(LoadNode(input));
  }

  return Node(move(result));
}

Node LoadDouble(istream &input) {
  double result = 0;
  bool flag = false;
  if (input.peek() == '-') {
    flag = true;
    input.get();
  }
  while (isdigit(input.peek())) {
    result *= 10;
    result += input.get() - '0';
  }
  if (input.peek() == '.') {
    double decimal_part = 0;
    double count = 1;
    input.get();
    while (isdigit(input.peek())) {
      decimal_part *= 10;
      count *= 10;
      decimal_part += input.get() - '0';
    }
    result += decimal_part / count;
  }
  if (flag) {
    result *= -1;
  }
  return Node(result);
}

Node LoadBool(istream &input) {
  string line;
  for (char c; input >> c && c != 'e';) {
    line.push_back(c);
  }
  return line == std::string("tru");
}

Node LoadString(istream &input) {
  string line;
  getline(input, line, '"');
  return Node(move(line));
}

Node LoadDict(istream &input) {
  map<string, Node> result;

  for (char c; input >> c && c != '}';) {
    if (c == ',') {
      input >> c;
    }

    string key = LoadString(input).AsString();
    input >> c;
    result.emplace(move(key), LoadNode(input));
  }

  return Node(move(result));
}

Node LoadNode(istream &input) {
  char c;
  input >> c;

  if (c == '[') {
    return LoadArray(input);
  } else if (c == '{') {
    return LoadDict(input);
  } else if (c == '"') {
    return LoadString(input);
  } else if (c == 't' || c == 'f') {
    input.putback(c);
    return LoadBool(input);
  } else /*if (Node::POSSIBLE_CHARS_FOR_DOUBLE.count(c) != 0)*/ {
    input.putback(c);
    return LoadDouble(input);
  }
}

Document Load(istream &input) { return Document{LoadNode(input)}; }

Document Load(const std::string &file_name) {
  std::ifstream in(file_name);
  if (in.is_open()) {
    return Document{LoadNode(in)};
  } else {
    throw std::runtime_error("failed to load from json file " + file_name);
  }
}
} // namespace Json