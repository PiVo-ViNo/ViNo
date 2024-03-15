#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "SymbolTable.h"
#include "TokenEnum.h"

namespace vino {

struct ExitAst;
struct StmtAst;
struct ScriptAst;
struct InsTypeAst;
struct InsideAst;
struct PersonaAst;
struct PersonaVarAst;
struct BackFileAst;
struct ForeFileAst;
struct ForePersonaAst;
struct TextLineAst;
struct TextFileAst;

/**
    @brief Visitor pattern-like struct
    @details Double dispatch method
    @param env_reference
*/
struct Visitor {
    explicit Visitor(SymbolTableEnv& symb_table_env, bool verb) :
        env_reference(symb_table_env), verbal(verb)
    {
    }

    void analyze(const ExitAst*) const;        
    void analyze(const ScriptAst*) const;      
    void analyze(const StmtAst*) const;        
    void analyze(const PersonaAst*) const;     
    void analyze(const InsideAst*) const;      
    void analyze(const InsTypeAst*) const;     
    void analyze(const PersonaVarAst*) const;  
    void analyze(const BackFileAst*) const;    
    void analyze(const ForeFileAst*) const;
    void analyze(const ForePersonaAst*) const;
    void analyze(const TextLineAst*) const;
    void analyze(const TextFileAst*) const;

    void true_analyze(const InsTypeAst*, Persona&) const;
    // referece instead of ptr, because no null possible (and must be)
    SymbolTableEnv& env_reference;
    bool verbal;
};

struct BasicAst {
    virtual void accept(const Visitor& visitor) const = 0;

    virtual ~BasicAst() {}
};

struct ExitAst : public BasicAst {
    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct StmtAst : public BasicAst {
    // PairTokenId               token;
    std::unique_ptr<StmtAst>  next_stmt;
    std::unique_ptr<BasicAst> expr;

    explicit StmtAst() : next_stmt(nullptr), expr(nullptr) {}

    explicit StmtAst(BasicAst* stmt_expr) : expr(stmt_expr) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct ScriptAst : public BasicAst {
    std::unique_ptr<StmtAst> stmt;
    // std::unique_ptr<ExitAst> ex;

    ScriptAst() : stmt(nullptr) {}

    explicit ScriptAst(StmtAst* nextAst /*, ExitAst* exitAst*/) :
        stmt(nextAst) /*, ex(exitAst)*/
    {
    }

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct InsTypeAst : public BasicAst {
    PairTokenId lexem;
    std::string str_param;

    InsTypeAst(const PairTokenId& token, const std::string& parameter) :
        lexem(token), str_param(parameter)
    {
    }

    explicit InsTypeAst(const PairTokenId& token) : InsTypeAst(token, "") {}

    explicit InsTypeAst(PairTokenId&& token) : InsTypeAst(token, "") {}

    InsTypeAst(PairTokenId&& token, std::string&& parameter) :
        lexem(token), str_param(parameter)
    {
    }

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct InsideAst : public BasicAst {
    std::unique_ptr<InsTypeAst> memb_type;
    std::unique_ptr<InsideAst>  next;

    InsideAst() : memb_type(nullptr), next(nullptr) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct PersonaAst : public BasicAst {
    const std::string          p_id;
    std::unique_ptr<InsideAst> inside;

    explicit PersonaAst(const std::string& persona_name_var) :
        p_id(persona_name_var), inside()
    {
    }

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct PersonaVarAst : public BasicAst {
    const std::string p_id;
    const std::string memb_id;
    const std::string param;

    PersonaVarAst(const std::string& persona_name,
                  const std::string& member_name, const std::string& textline) :

        p_id(persona_name), memb_id(member_name), param(textline)
    {
    }

    PersonaVarAst(std::string&& persona_name, std::string&& member_name,
                  std::string&& textline) :

        p_id(std::move(persona_name)),
        memb_id(std::move(member_name)),
        param(std::move(textline))
    {
    }

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct BackFileAst : public BasicAst {
    const std::string path_bg;

    explicit BackFileAst(const std::string& path_to_bg) : path_bg(path_to_bg) {}

    explicit BackFileAst(std::string&& path_to_bg) : path_bg(path_to_bg) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct ForeFileAst : public BasicAst {
    const std::string path_fg;

    explicit ForeFileAst(const std::string& path_to_fg) : path_fg(path_to_fg) {}

    explicit ForeFileAst(std::string&& path_to_fg) : path_fg(path_to_fg) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct ForePersonaAst : public BasicAst {
    const std::string p_id;
    const std::string memb_name;

    ForePersonaAst(const std::string& persona_name,
                   const std::string& member_name) :

        p_id(persona_name), memb_name(member_name)
    {
    }

    ForePersonaAst(std::string&& persona_name, std::string&& member_name) :

        p_id(std::move(persona_name)), memb_name(std::move(member_name))
    {
    }

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct TextLineAst : public BasicAst {
    const std::string text;

    explicit TextLineAst(const std::string& text) : text(text) {}

    explicit TextLineAst(std::string&& text) : text(text) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

struct TextFileAst : public BasicAst {
    const std::string path_txt;

    explicit TextFileAst(const std::string& path_to_txt) : path_txt(path_to_txt)
    {
    }

    explicit TextFileAst(std::string&& path_to_txt) : path_txt(path_to_txt) {}

    inline void accept(const Visitor& visitor) const override
    {
        return visitor.analyze(this);
    }
};

}  // namespace vino
