#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "TokenEnum.h"

namespace vino {

enum class NodeType {
    empty,
    script,
    stmt,
    bg,
    fg_file,
    fg_pers,
    txt_line,
    txt_file,
    persona,
    inside,
    inside_type,
    pers_var,
    exit
};

struct Visitor;

struct BasicAst {
    NodeType type;

    explicit BasicAst(NodeType type) : type(type) {}
    // R accept(Visitor visitor);
};

struct ExitAst : public BasicAst {
    ExitAst() : BasicAst(NodeType::exit) {}
};

struct StmtAst : public BasicAst {
    // PairTokenId               token;
    std::shared_ptr<StmtAst>  next_stmt;
    std::unique_ptr<BasicAst> expr;

    explicit StmtAst() :
        BasicAst(NodeType::stmt), next_stmt(nullptr), expr(nullptr)
    {
    }

    explicit StmtAst(BasicAst* stmt_expr) :
        BasicAst(NodeType::stmt), expr(stmt_expr)
    {
    }
};

struct ScriptAst : public BasicAst {
    std::shared_ptr<StmtAst> stmt;
    // std::unique_ptr<ExitAst> ex;

    ScriptAst() : BasicAst(NodeType::script), stmt(nullptr) {}

    explicit ScriptAst(StmtAst* nextAst /*, ExitAst* exitAst*/) :
        BasicAst(NodeType::script), stmt(nextAst) /*, ex(exitAst)*/
    {
    }
};

struct InsTypeAst : public BasicAst {
    PairTokenId token;
    std::string str_param;

    InsTypeAst(const PairTokenId& token, const std::string& parameter) :
        BasicAst(NodeType::inside_type), token(token), str_param(parameter)
    {
    }

    explicit InsTypeAst(PairTokenId&& token) :
        InsTypeAst(token, "")
    {
    }

    InsTypeAst(PairTokenId&& token, std::string&& parameter) :
        BasicAst(NodeType::inside_type), token(token), str_param(parameter)
    {
    }
};

struct InsideAst : public BasicAst {
    std::unique_ptr<InsTypeAst> memb_type;
    std::shared_ptr<InsideAst>  next;

    InsideAst() : BasicAst(NodeType::inside), memb_type(nullptr), next(nullptr)
    {
    }
};

struct PersonaAst : public BasicAst {
    const std::string          p_name;
    std::unique_ptr<InsideAst> inside;

    explicit PersonaAst(const std::string& persona_name_var) :
        BasicAst(NodeType::persona), p_name(persona_name_var), inside()
    {
    }
};

struct PersonaVarAst : public BasicAst {
    const std::string p_name;
    const std::string memb_name;
    const std::string text;

    PersonaVarAst(const std::string& persona_name,
                  const std::string& member_name, const std::string& textline) :
        BasicAst(NodeType::pers_var),
        p_name(persona_name),
        memb_name(member_name),
        text(textline)
    {
    }

    PersonaVarAst(std::string&& persona_name, std::string&& member_name,
                  std::string&& textline) :
        BasicAst(NodeType::pers_var),
        p_name(std::move(persona_name)),
        memb_name(std::move(member_name)),
        text(std::move(textline))
    {
    }
};

struct BackFileAst : public BasicAst {
    const std::string path_bg;

    explicit BackFileAst(const std::string& path_to_bg) :
        BasicAst(NodeType::bg), path_bg(path_to_bg)
    {
    }

    explicit BackFileAst(std::string&& path_to_bg) :
        BasicAst(NodeType::bg), path_bg(path_to_bg)
    {
    }
};

struct ForeFileAst : public BasicAst {
    const std::string path_fg;

    explicit ForeFileAst(const std::string& path_to_fg) :
        BasicAst(NodeType::fg_file), path_fg(path_to_fg)
    {
    }

    explicit ForeFileAst(std::string&& path_to_fg) :
        BasicAst(NodeType::fg_file), path_fg(path_to_fg)
    {
    }
};

struct ForePersonaAst : public BasicAst {
    const std::string p_name;
    const std::string memb_name;

    ForePersonaAst(const std::string& persona_name,
                   const std::string& member_name) :
        BasicAst(NodeType::fg_pers),
        p_name(persona_name),
        memb_name(member_name)
    {
    }

    ForePersonaAst(std::string&& persona_name, std::string&& member_name) :
        BasicAst(NodeType::fg_pers),
        p_name(std::move(persona_name)),
        memb_name(std::move(member_name))
    {
    }
};

struct TextLineAst : public BasicAst {
    const std::string text;

    explicit TextLineAst(const std::string& text) :
        BasicAst(NodeType::txt_line), text(text)
    {
    }

    explicit TextLineAst(std::string&& text) :
        BasicAst(NodeType::txt_line), text(text)
    {
    }
};

struct TextFileAst : public BasicAst {
    const std::string path_txt;

    explicit TextFileAst(const std::string& path_to_txt) :
        BasicAst(NodeType::txt_file), path_txt(path_to_txt)
    {
    }

    explicit TextFileAst(std::string&& path_to_txt) :
        BasicAst(NodeType::txt_file), path_txt(path_to_txt)
    {
    }
};

struct Visitor {
    std::string visit();
};
}  // namespace vino
