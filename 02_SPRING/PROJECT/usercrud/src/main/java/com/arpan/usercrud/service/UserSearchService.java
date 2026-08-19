package com.arpan.usercrud.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.arpan.usercrud.model.QUser;
import com.arpan.usercrud.model.User;
import com.querydsl.core.BooleanBuilder;
import com.querydsl.jpa.impl.JPAQueryFactory;

// ═══════════════════════════════════════════════════════════════════════
// FLEXIBLE SEARCH — QueryDSL way (type-safe dynamic query)
//    @Query string NAHI, Streams NAHI — Q-classes + method chaining, compiler-checked.
//
// >>> TERA TASK: search() ke andar WHERE conditions likhna (BooleanBuilder se). <<<
//    QUser user = QUser.user;                    <- generated Q-class (compile-time safe)
//    BooleanBuilder where = new BooleanBuilder(); <- yahaan conditions AND-jodo
//
//    conditions (optional — null ho to skip):
//       role   != null  ->  where.and(user.role.eq(role));
//       minAge != null  ->  where.and(user.age.goe(minAge));   // goe = greater-or-equal (>=)
//
//    ★ NOTE type-safe hai: user.role, user.age COMPILE-TIME pe check hote
//      (string "role" galat likhne ka darr nahi — @Query se yahi farak).
// ═══════════════════════════════════════════════════════════════════════

@Service
public class UserSearchService {

    private final JPAQueryFactory queryFactory;

    public UserSearchService(JPAQueryFactory queryFactory) {
        this.queryFactory = queryFactory;
    }

    public List<User> search(String role, Integer minAge) {
        QUser user = QUser.user;
        BooleanBuilder where = new BooleanBuilder();

        if(role != null) {
            where.and(user.role.eq(role));
        }
        if(minAge != null) {
            where.and(user.age.goe(minAge));
        }

        // final query — where + fetch (ye ready hai)
        return queryFactory
                .selectFrom(user)
                .where(where)
                .fetch();
    }
}
