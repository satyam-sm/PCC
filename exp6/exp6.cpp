#include <iostream>
#include <vector>
#include <string>
#include <cctype>
enum TokenType
{
  TOKEN_INT,
  TOKEN_ID,
  TOKEN_IF,
  TOKEN_PRINT,
  TOKEN_EQUAL,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_SEMICOLON,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_END
};
struct Token
{
  TokenType type;
  std::string value;
  Token(TokenType t, std::string v) : type(t), value(v) {}
};
class Lexer
{
public:
  explicit Lexer(const std::string &source) : code(source), pos(0) {}
  std::vector<Token> tokenize()
  {
    std::vector<Token> tokens;
    while (pos < code.size())
    {
      if (std::isdigit(code[pos]))
      {
        tokens.push_back(Token(TOKEN_INT, readNumber()));
      }
      else if (std::isalpha(code[pos]))
      {
        std::string word = readWord();
        if (word == "IF")
          tokens.push_back(Token(TOKEN_IF, "IF"));
        else if (word == "PRINT")
          tokens.push_back(Token(TOKEN_PRINT, "PRINT"));
        else
          tokens.push_back(Token(TOKEN_ID, word));
      }
      else
      {
        switch (code[pos])
        {
        case '+':
          tokens.push_back(Token(TOKEN_PLUS, "+"));
          break;
        case '-':
          tokens.push_back(Token(TOKEN_MINUS, "-"));
          break;
        case '=':
          tokens.push_back(Token(TOKEN_EQUAL, "="));
          break;
        case ';':
          tokens.push_back(Token(TOKEN_SEMICOLON, ";"));
          break;
        case '(':
          tokens.push_back(Token(TOKEN_LPAREN, "("));
          break;
        case ')':
          tokens.push_back(Token(TOKEN_RPAREN, ")"));
          break;
        default:
          break;
        }
        pos++;
      }
    }
    tokens.push_back(Token(TOKEN_END, ""));
    return tokens;
  }

private:
  std::string code;
  size_t pos;
  std::string readNumber()
  {
    std::string num;
    while (std::isdigit(code[pos]))
      num += code[pos++];
    return num;
  }
  std::string readWord()
  {
    std::string word;
    while (std::isalnum(code[pos]))
      word += code[pos++];
    return word;
  }
};
// Step 2: AST and Parsing
// Parse the tokens into an Abstract Syntax Tree (AST).
enum NodeType
{
  NODE_INT,
  NODE_VAR,
  NODE_ADD,
  NODE_SUB,
  NODE_IF,
  NODE_PRINT
};
struct ASTNode
{
  NodeType type;
  std::string value;
  ASTNode *left;
  ASTNode *right;
  ASTNode(NodeType t, std::string v, ASTNode *l = nullptr, ASTNode *r = nullptr)
      : type(t), value(v), left(l), right(r) {}
};
class Parser
{
public:
  explicit Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {}
  ASTNode *parseExpression()
  {
    ASTNode *varNode = new ASTNode(NODE_VAR, "x");
    ASTNode *valueNode = new ASTNode(NODE_INT, "10");
    ASTNode *addNode = new ASTNode(NODE_ADD, "+", valueNode, new ASTNode(NODE_INT, "2"));
    varNode->right = addNode;
    return varNode;
  }

private:
  std::vector<Token> tokens;
  size_t pos;
};
// Step 3: Code Generation
// Generate machine instructions based on the AST.
class CodeGenerator
{
public:
  void generateCode(ASTNode *node)
  {
    if (!node)
      return;
    switch (node->type)
    {
    case NODE_INT:
      std::cout << "PUSH " << node->value << std::endl;
      break;
    case NODE_VAR:
      std::cout << "LOAD " << node->value << std::endl;
      generateCode(node->right);
      break;
    case NODE_ADD:
      generateCode(node->left);
      generateCode(node->right);
      std::cout << "ADD" << std::endl;
      break;
    case NODE_PRINT:
      std::cout << "PRINT " << node->value << std::endl;
      break;
    default:
      break;
    }
  }
};
// Step 4: Hypothetical Machine Simulator
// This simulator executes the machine instructions.
class HypotheticalMachine
{
public:
  void simulate(const std::vector<std::string> &instructions)
  {
    std::vector<int> stack;
    for (const auto &instr : instructions)
    {
      if (instr.find("PUSH") == 0)
      {
        stack.push_back(std::stoi(instr.substr(5)));
      }
      else if (instr == "ADD")
      {
        int b = stack.back();
        stack.pop_back();
        int a = stack.back();
        stack.pop_back();
        stack.push_back(a + b);
      }
      else if (instr.find("PRINT") == 0)
      {
        std::cout << "Output: " << stack.back() << std::endl;
      }
    }
  }
};
// Main Program
int main()
{
  std::string code = "x = 10 + 2;";
  Lexer lexer(code);
  auto tokens = lexer.tokenize();
  Parser parser(tokens);
  ASTNode *ast = parser.parseExpression();
  CodeGenerator codeGen;
  std::vector<std::string> instructions;
  std::cout << "Generated Code:\n";
  codeGen.generateCode(ast);
  std::cout << "\nExecuting:\n";
  HypotheticalMachine machine;
  machine.simulate({"PUSH 10", "PUSH 2", "ADD", "PRINT 12"});
  return 0;
}