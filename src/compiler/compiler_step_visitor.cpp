/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#include "kernel/instantiate.h"
#include "compiler/compiler_step_visitor.h"

namespace lean {
compiler_step_visitor::compiler_step_visitor(environment const & env):
    m_env(env),
    m_ctx(m_env, options(), m_mctx, m_lctx, transparency_mode::All) {
}

compiler_step_visitor::~compiler_step_visitor() {
}

expr compiler_step_visitor::visit_lambda_let(expr const & e) {
    type_context::tmp_locals locals(m_ctx);
    expr t = e;
    while (true) {
        /* Types are ignored in compilation steps. So, we do not invoke visit for d. */
        if (is_lambda(t)) {
            expr d = instantiate_rev(binding_domain(t), locals.size(), locals.data());
            locals.push_local(binding_name(t), d, binding_info(t));
            t = binding_body(t);
        } else if (is_let(t)) {
            expr d = instantiate_rev(let_type(t), locals.size(), locals.data());
            expr v = visit(instantiate_rev(let_value(t), locals.size(), locals.data()));
            locals.push_let(let_name(t), d, v);
            t = let_body(t);
        } else {
            break;
        }
    }
    t = instantiate_rev(t, locals.size(), locals.data());
    t = visit(t);
    return locals.mk_lambda(t);
}

expr compiler_step_visitor::visit_lambda(expr const & e) {
    return visit_lambda_let(e);
}

expr compiler_step_visitor::visit_let(expr const & e) {
    return visit_lambda_let(e);
}
}