package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import com.javatpoint.model.Claim;
import com.javatpoint.model.Policy;

// REPOSITORY: policy ka DB-access + custom @Query. CrudRepository -> CRUD auto.
public interface PolicyRepository extends CrudRepository<Policy, Integer> {

	@Query(nativeQuery = true, value = "select id,policies from policy")
	public List<Policy> getAllPolices();                          // saari policies (id, policies)

	@Query(nativeQuery = true, value = "SELECT * FROM policy WHERE id = :id")
	public List<Policy> getAllPoliciesById(@Param("id") int id);  // us id ke rows (List)

	@Query(nativeQuery = true, value = "SELECT * FROM policy WHERE id = :id")
	public Policy getPoliciesById(@Param("id") int id);           // ek Policy (single)

}
