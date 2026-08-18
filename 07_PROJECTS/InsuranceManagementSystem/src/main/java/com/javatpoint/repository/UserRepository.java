package com.javatpoint.repository;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.javatpoint.model.User;

// REPOSITORY: user ka DB-access + AUTH queries. CrudRepository -> CRUD auto.
@Repository
public interface UserRepository extends CrudRepository<User, Integer> {
	// DERIVED query (method-NAME se Spring khud SQL banata, @Query nahi chahiye):
	Optional<User> findOneByEmailAndPassword(String email,String password);   // login-check
	User findByEmail(String email);                                           // email se user

	@Query(value = "select * from user where last_login<=:time and email_sent=false",nativeQuery = true)
	List<User> getUserLoggedIn24HoursAgo(@Param("time") LocalDateTime time);  // 24h se inactive (reminder-email ko)

	@Query(value = "select * from user where verification_token=:token",nativeQuery = true)
	User findByVerificationToken(@Param("token") String token);               // email-verify token se user



}
