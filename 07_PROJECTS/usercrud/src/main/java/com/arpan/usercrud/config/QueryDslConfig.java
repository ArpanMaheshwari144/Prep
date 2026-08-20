package com.arpan.usercrud.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.querydsl.jpa.impl.JPAQueryFactory;

import jakarta.persistence.EntityManager;
import jakarta.persistence.PersistenceContext;

// ═══════════════════════════════════════════════════════════════════════
// QueryDSL config — JPAQueryFactory bean banata (EntityManager se).
//   Ye factory se hum type-safe queries chalate: queryFactory.selectFrom(QUser)...
//   (plumbing — ise chhedna nahi.)
// ═══════════════════════════════════════════════════════════════════════

@Configuration
public class QueryDslConfig {

    @PersistenceContext
    private EntityManager entityManager;

    @Bean
    public JPAQueryFactory jpaQueryFactory() {
        return new JPAQueryFactory(entityManager);
    }
}
