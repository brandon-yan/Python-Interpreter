#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include <stack>
#include <vector>
#include <cstring>
#include <iomanip>
#include "bigint.h"
#include "Python3BaseVisitor.h"
#include "Python3Parser.h"


class EvalVisitor: public Python3BaseVisitor {

//todo:override all methods of Python3BaseVisitor
struct argumen {
  std::string nam;
  alltype tes;
};
struct flowstmt {
  std::string flow;
  std::vector<alltype> retlist;
};
struct function {
  std::string nam;
  std::vector<argumen> typelist;
  Python3Parser::SuiteContext *suit;
};
std::stack<std::map<std::string, alltype>> mystack;
std::map<std::string, alltype> *stoa;
std::map<std::string, function> myfun;
virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
    function func;
    func.nam = ctx->NAME()->toString();
    antlrcpp::Any tmp = visitParameters(ctx->parameters());
    if (tmp.is<std::vector<argumen>>())
      func.typelist = tmp.as<std::vector<argumen>>();
    func.suit = ctx->suite();
    myfun[func.nam] = func;
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
    if (ctx->typedargslist() != nullptr) return visitTypedargslist(ctx->typedargslist());
  }

  virtual antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
    std::vector<argumen> type1;
    std::vector<argumen> type2;
    int len1 = ctx->tfpdef().size() - 1, len2 = ctx->test().size() - 1;
    for (int i = len1; i >= 0 ; --i) {
      argumen tmp;
      tmp.nam = visitTfpdef(ctx->tfpdef(i)).as<std::string>();
      if (ctx->test(len2) != nullptr) {
        tmp.tes = visitTest(ctx->test(len2)).as<alltype>();
        len2--;
      }
      type1.push_back(tmp);
    }
    for (int i = type1.size() - 1; i >= 0; --i) type2.push_back(type1[i]);
    return type2;
  }

  virtual antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {       
    std::string ret = ctx->NAME()->toString();
    return ret;
  }

  virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
      if (ctx->simple_stmt() != nullptr) return visitSimple_stmt(ctx->simple_stmt());
      else if (ctx->compound_stmt() != nullptr) return visitCompound_stmt(ctx->compound_stmt());
  }

  virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
    return visitSmall_stmt(ctx->small_stmt());
  }

  virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
    if (ctx->expr_stmt() != nullptr) return visitExpr_stmt(ctx->expr_stmt());
    else if (ctx->flow_stmt() != nullptr) return visitFlow_stmt(ctx->flow_stmt());
  }

  virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
    if (ctx->testlist().size() == 1) return visitTestlist(ctx->testlist(0));
    else if (ctx->augassign() != nullptr) {
      std::vector<alltype> tmp1 = visitTestlist(ctx->testlist(0)).as<std::vector<alltype>>();
      std::vector<alltype> tmp2 = visitTestlist(ctx->testlist(1)).as<std::vector<alltype>>();
      std::string auga = visitAugassign(ctx->augassign()).as<std::string>();
      if (auga == "+=") mystack.top()[tmp1[0].name] = mystack.top()[tmp1[0].name] + tmp2[0];
      else if (auga == "-=") mystack.top()[tmp1[0].name] = mystack.top()[tmp1[0].name] - tmp2[0];
      else if (auga == "*=") mystack.top()[tmp1[0].name] = mystack.top()[tmp1[0].name] * tmp2[0];     
      else if (auga == "/=") mystack.top()[tmp1[0].name] = mystack.top()[tmp1[0].name] / tmp2[0];
      else if (auga == "//=") mystack.top()[tmp1[0].name] = intdivide(mystack.top()[tmp1[0].name], tmp2[0]);
      else if (auga == "%=") mystack.top()[tmp1[0].name] = mystack.top()[tmp1[0].name] % tmp2[0];
      return nullptr;
    }
    else if (ctx->ASSIGN().size() != 0) {
      int len = ctx->testlist().size();
      std::vector<alltype> val = visitTestlist(ctx->testlist(len - 1)).as<std::vector<alltype>>();
      for (int i = 0; i < ctx->testlist().size(); ++i) {
        std::vector<alltype> tmp = visitTestlist(ctx->testlist(i)).as<std::vector<alltype>>();
        for (int j = 0; j < tmp.size(); ++j) mystack.top()[tmp[j].name] = val[j];
      }
      return nullptr;
    }
  }

  virtual antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
    if (ctx->ADD_ASSIGN() != nullptr) return (std::string)"+=";
    else if (ctx->SUB_ASSIGN() != nullptr) return (std::string)"-=";
    else if (ctx->MULT_ASSIGN() != nullptr) return (std::string)"*=";
    else if (ctx->DIV_ASSIGN() != nullptr) return (std::string)"/=";
    else if (ctx->IDIV_ASSIGN() != nullptr) return (std::string)"//=";
    else if (ctx->MOD_ASSIGN() != nullptr) return (std::string)"%=";
  }

  virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
      if (ctx->break_stmt() != nullptr) return visitBreak_stmt(ctx->break_stmt());
      else if (ctx->continue_stmt() != nullptr) return visitContinue_stmt(ctx->continue_stmt());
      else if (ctx->return_stmt() != nullptr) return visitReturn_stmt(ctx->return_stmt());
  }

  virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
    flowstmt flo;
    flo.flow = (std::string)"break";
    return flo;
  }

  virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    flowstmt flo;
    flo.flow = (std::string)"continue";
    return flo;
  }

  virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    flowstmt flo;
    flo.flow = (std::string)"return";
    if (ctx->testlist() != nullptr) flo.retlist = visitTestlist(ctx->testlist()).as<std::vector<alltype>>(); 
    return flo;
  }

  virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
    if (ctx->if_stmt() != nullptr) return visitIf_stmt(ctx->if_stmt());
    else if (ctx->while_stmt() != nullptr) return visitWhile_stmt(ctx->while_stmt());
    else if (ctx->funcdef() != nullptr) return visitFuncdef(ctx->funcdef());
  }

  virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
    for (int i = 0; i < ctx->test().size(); ++i) {
      alltype ifstmt = visitTest(ctx->test(i)).as<alltype>();
      if (ifstmt.booval == 1) {
        antlrcpp::Any ifflow = visitSuite(ctx->suite(i));
        if (ifflow.is<flowstmt>()) return ifflow;
        break;
      }
    }
    return nullptr;
  }

  virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
    alltype whiletest = visitTest(ctx->test()).as<alltype>();
    while (whiletest.booval) {
      antlrcpp::Any whilesuit = visitSuite(ctx->suite());
      if (whilesuit.is<flowstmt>()) {
        flowstmt tmp = whilesuit.as<flowstmt>();
        if (tmp.flow == (std::string)"break") break;
        if (tmp.flow == (std::string)"continue") continue;
        if (tmp.flow == (std::string)"return") return tmp.retlist;
      }
      whiletest = visitTest(ctx->test()).as<alltype>();
    }
    return nullptr;
  }

  virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
    if (ctx->simple_stmt() != nullptr) {
      std::cout << (int)1 << std::endl;
      antlrcpp::Any suit = visitSimple_stmt(ctx->simple_stmt());
      if (suit.is<flowstmt>()) {
        flowstmt tmp = suit.as<flowstmt>();
        std::cout << tmp.flow << std::endl;
        return tmp;
      }
    }  
    else {
      for (int i = 0; i < ctx->stmt().size(); ++i) {
        antlrcpp::Any stm = visitStmt(ctx->stmt(i));
        if (stm.is<flowstmt>()) {
          flowstmt tmp = stm.as<flowstmt>();
          return tmp;
        }
      }
    }
    return nullptr;
  }

  virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
    return visitOr_test(ctx->or_test());
  }

  virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
    if (ctx->and_test().size() == 1) return visitAnd_test(ctx->and_test(0));
    else {
      alltype ret = visitAnd_test(ctx->and_test(0)).as<alltype>();
      for (int i = 1; i < ctx->and_test().size(); ++i) {
        if (ret.booval == true) break;
        alltype tmp = visitAnd_test(ctx->and_test(i)).as<alltype>();
        bool x1 = ret.toBOOL(), x2 = tmp.toBOOL();
        ret.booval = x1 || x2;
      }
      return ret;
    }
  }

  virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
    if (ctx->not_test().size() == 1) return visitNot_test(ctx->not_test(0));
    else {
      alltype ret = visitNot_test(ctx->not_test(0)).as<alltype>();
      for (int i = 1; i < ctx->not_test().size(); ++i) {
        if (ret.booval == false) break;
        alltype tmp = visitNot_test(ctx->not_test(i)).as<alltype>();
        bool x1 = ret.toBOOL(), x2 = tmp.toBOOL();
        ret.booval = x1 && x2;
      }
      return ret;
    }
  }

  virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
    if (ctx->comparison() != nullptr) return visitComparison(ctx->comparison());
    if (ctx->not_test() != nullptr) {
      alltype tmp = visitNot_test(ctx->not_test()).as<alltype>();
      bool x1 = tmp.toBOOL();
      tmp.booval = !x1;
      return tmp;
    }
  }

  virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
    if (ctx->arith_expr().size() == 1) return visitArith_expr(ctx->arith_expr(0));
    else {
      std::vector <alltype> bb;
      alltype tmp1 = true, tmp2 = true;
      //std::cout << tmp1.booval << std::endl;
      for (int i = 0; i < ctx->arith_expr().size() - 1; ++i) {
        std::string comop = visitComp_op(ctx->comp_op(i)).as<std::string>();
        alltype x1 = visitArith_expr(ctx->arith_expr(i)).as<alltype>(), x2 = visitArith_expr(ctx->arith_expr(i + 1)).as<alltype>();
        if (comop == "<") tmp1 = x1 < x2;
        else if (comop == ">") tmp1 = x1 > x2;
        else if (comop == "==") tmp1 = x1 == x2;        
        else if (comop == ">=") tmp1 = x1 >= x2;
        else if (comop == "<=") tmp1 = x1 <= x2;
        else if (comop == "!=") tmp1 = x1 != x2;
        bb.push_back(tmp1);
      }
      for (int i = 0; i < bb.size(); ++i) {
        tmp2 = tmp2 && bb[i];
        if (tmp2.booval == false) break;
      }
      return tmp2;
    }
  }

  virtual antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
    if (ctx->LESS_THAN() != nullptr) return (std::string)"<";
    else if (ctx->GREATER_THAN() != nullptr) return (std::string)">";
    else if (ctx->EQUALS() != nullptr) return (std::string)"==";
    else if (ctx->GT_EQ() != nullptr) return (std::string)">=";
    else if (ctx->LT_EQ() != nullptr) return (std::string)"<=";
    else if (ctx->NOT_EQ_2() != nullptr) return (std::string)"!=";
  } 

  virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    if (ctx->term().size() == 1) return visitTerm(ctx->term(0));
    else {
      alltype tmp = visitTerm(ctx->term(0));
      std::string oper = "0";
      int x;
      for (int i = 0; i < 100000; ++i) oper = oper + "0";
      for (int i = 0; i < ctx->ADD().size(); ++i) {
        x = ctx->ADD(i)->getSymbol()->getTokenIndex();
        oper[x] = '+';
      } 
      for (int i = 0; i < ctx->MINUS().size(); ++i) {
        x = ctx->MINUS(i)->getSymbol()->getTokenIndex();
        oper[x] = '-';
      }
      oper.erase(remove(oper.begin(), oper.end(), '0'), oper.end());
      for (int i = 1; i < ctx->term().size(); ++i) {
        if (oper[i - 1] == '+') tmp = tmp + visitTerm(ctx->term(i));
        if (oper[i - 1] == '-') tmp = tmp - visitTerm(ctx->term(i));
      }
      return tmp;
    }
  }

  virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
    if (ctx->factor().size() == 1) return visitFactor(ctx->factor(0));
    else {
      alltype tmp = visitFactor(ctx->factor(0)).as<alltype>();
      std::string oper = "0";
      int x;
      for (int i = 0; i < 100000; ++i) oper = oper + "0";
      for (int i = 0; i < ctx->STAR().size(); ++i) {
        x = ctx->STAR(i)->getSymbol()->getTokenIndex();
        oper[x] = '*';
      } 
      for (int i = 0; i < ctx->DIV().size(); ++i) {
        x = ctx->DIV(i)->getSymbol()->getTokenIndex();
        oper[x] = '/';
      }
      for (int i = 0; i < ctx->IDIV().size(); ++i) {
        x = ctx->IDIV(i)->getSymbol()->getTokenIndex();
        oper[x] = 'i';
      }
      for (int i = 0; i < ctx->MOD().size(); ++i) {
        x = ctx->MOD(i)->getSymbol()->getTokenIndex();
        oper[x] = '%';
      }
      oper.erase(remove(oper.begin(), oper.end(), '0'), oper.end());
      for (int i = 1; i < ctx->factor().size(); ++i) {
        if (oper[i - 1] == '*') tmp = tmp * visitFactor(ctx->factor(i));
        if (oper[i - 1] == '/') tmp = tmp / visitFactor(ctx->factor(i));
        if (oper[i - 1] == 'i') tmp = intdivide(tmp, visitFactor(ctx->factor(i)));
        if (oper[i - 1] == '%') tmp = tmp % visitFactor(ctx->factor(i));
      }
      return tmp;
    }
  }

  virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
    if (ctx->atom_expr() != nullptr) return visitAtom_expr(ctx->atom_expr());
    else {
      if (ctx->ADD() != nullptr) return visitFactor(ctx->factor());
      else if (ctx->MINUS() != nullptr) {
        alltype tmp = ((Bigint)-1) * visitFactor(ctx->factor()).as<alltype>();
        return tmp;
      }
    }
  }

  virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
    if (ctx->trailer() != nullptr) {
      alltype tmp = visitAtom(ctx->atom()).as<alltype>();
      std::vector<argumen> arglis = visitTrailer(ctx->trailer()).as<std::vector<argumen>>();
      if (tmp.name == "print") {
        for (int i = 0; i < arglis.size(); ++i) {
          alltype ret = arglis[i].tes;
          if (ret.name == "") {
            if (ret.Type == INT) std::cout << (std::string)ret.intval << " ";
            if (ret.Type == DOUBLE) std::cout << std::fixed << std::setprecision(6) << ret.douval << " ";
            if (ret.Type == STRING) std::cout << ret.strval << " ";
            if (ret.Type == BOOL) {
              if (ret.booval == 1 ) std::cout << "True" << " ";
              else std::cout << "False" << " ";
            }
            if (ret.Type == NONE) std::cout << (std::string)"None" << " ";
          }
          else {
            alltype ret1 = mystack.top()[ret.name];
            if (ret1.Type == INT) std::cout << (std::string)ret1.intval << " ";
            if (ret1.Type == DOUBLE) std::cout << std::fixed << std::setprecision(6) << ret1.booval << " ";
            if (ret1.Type == STRING) std::cout << ret1.strval << " ";
            if (ret.Type == BOOL) {
              if (ret.booval == 1 ) std::cout << "True" << " ";
              else std::cout << "False" << " ";
            }
            if (ret.Type == NONE) std::cout << (std::string)"None" << " ";
          }
        }
        std::cout << std::endl;
      }
      if (tmp.name == "int") {
        alltype ret = arglis[0].tes;
        Bigint ret1 = ret.toINT();
        alltype ret2 = ret1;
        return ret2;
      }
      if (tmp.name == "float") {
        alltype ret = arglis[0].tes;
        double ret1 = ret.toDOUBLE();
        alltype ret2 = ret1;
        return ret2;
      }
      if (tmp.name == "string") {
        alltype ret = arglis[0].tes;
        std::string ret1 = ret.toSTRING();
        alltype ret2 = ret1;
        return ret2;
      }
      if (tmp.name == "bool") {
        alltype ret = arglis[0].tes;
        bool ret1 = ret.toBOOL();
        alltype ret2 = ret1;
        return ret2;
      }
      
    }
    return visitAtom(ctx->atom());
  }

  virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
    if (ctx->arglist() != nullptr) return visitArglist(ctx->arglist());
  }

  virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
    if (ctx->NAME() != nullptr) {
      std::string tmp = ctx->NAME()->toString();
      if (mystack.top().count(tmp) == 0) {
        alltype ret;
        ret.name = ctx->NAME()->toString();
        return ret;
      }
      else {
        alltype ret = mystack.top()[ctx->NAME()->toString()];
        ret.name = ctx->NAME()->toString();
        return ret;
      }
    }
    else if (ctx->NUMBER() != nullptr) {
      std::string ret = ctx->NUMBER()->toString();
      auto ifdou = ret.find(".");
      alltype tmp = ret;
      if (ifdou == string::npos) {
        alltype ret1 = tmp.toINT();
        return ret1;
      }
      else {
        alltype ret1 = tmp.toDOUBLE();
        return ret1;
      }
    }
    else if (!ctx->STRING().empty()) {
      std::string ret1;
      for (int i = 0; i < ctx->STRING().size(); ++i) {
        std::string tmp = ctx->STRING(i)->toString();
        ret1 = ret1 + std::string(tmp, 1, tmp.length() - 2);
      }
      alltype ret = ret1;
      return ret;
    }
    else if (ctx->NONE() != nullptr) {
      alltype ret;
      return ret;
    }
    else if (ctx->TRUE() != nullptr) {
      alltype ret = "True";
      return ret;
    }
    else if (ctx->FALSE() != nullptr) {
      alltype ret = "False";
      return ret;
    }
    return visit(ctx->test());
  }

  virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
    std::vector<alltype> tmp;
    for (int i = 0; i < ctx->test().size(); ++i)
      tmp.push_back(visitTest(ctx->test(i)).as<alltype>());
    return tmp;
  }

  virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
    std::vector<argumen> arg;
    for (int i = 0; i < ctx->argument().size(); ++i) {
      argumen argu = visitArgument(ctx->argument(i)).as<argumen>();
      arg.push_back(argu);
    }
    return arg; 
  }

  virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
    argumen argu;
    if (ctx->NAME() != nullptr) {
      argu.nam = ctx->NAME()->toString();
      argu.tes = visitTest(ctx->test()).as<alltype>();
    }
    else argu.tes = visitTest(ctx->test()).as<alltype>();
    return argu;
  }

};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H