package com.arpan.usercrud.controller;

import java.util.List;

import org.springframework.graphql.data.method.annotation.Argument;
import org.springframework.graphql.data.method.annotation.QueryMapping;
import org.springframework.stereotype.Controller;

import com.arpan.usercrud.model.User;
import com.arpan.usercrud.repository.UserRepository;

// ═══════════════════════════════════════════════════════════════════════
// GraphQL RESOLVER — schema ke queries ka data yahan se aata.
//    @Controller (NA @RestController — GraphQL alag hai).
//    @QueryMapping method = schema ke Query-field se JUD jaata (naam SAME hona chahiye):
//        schema:  users        -> method: users()
//        schema:  user(id)     -> method: user(@Argument ... id)
//    @Argument = query ka argument (id) method-param me bind karta.
//
// >>> TERA TASK: neeche 2 method ke BODY likhna (repo se data laa — jo tu jaanta). <<<
// ═══════════════════════════════════════════════════════════════════════

@Controller
public class UserGraphQLController {

    private final UserRepository userRepository;

    public UserGraphQLController(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    // schema: users: [User!]!   -> saare users
    @QueryMapping
    public List<User> users() {
        return userRepository.findAll();
    }

    // schema: user(id: ID!): User   -> ek user by id
    @QueryMapping
    public User user(@Argument Long id) {
        return userRepository.findById(id).orElse(null);
    }
}
