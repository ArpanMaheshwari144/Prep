package com.arpan.usercrud.controller;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.arpan.usercrud.model.User;
import com.arpan.usercrud.service.UserSearchService;

// ═══════════════════════════════════════════════════════════════════════
// FLEXIBLE SEARCH — QueryDSL way
//    GET /api/users/search?role=USER&minAge=25
//    params -> @RequestParam (optional) -> service (QueryDSL) -> List<User>
//    (controller READY — tera kaam service ke andar QueryDSL likhna hai.)
// ═══════════════════════════════════════════════════════════════════════

@RestController
@RequestMapping("/api/users")
public class UserSearchController {

    private final UserSearchService userSearchService;

    public UserSearchController(UserSearchService userSearchService) {
        this.userSearchService = userSearchService;
    }

    @GetMapping("/search")
    public List<User> search(
            @RequestParam(required = false) String role,
            @RequestParam(required = false) Integer minAge) {
        return userSearchService.search(role, minAge);
    }
}
