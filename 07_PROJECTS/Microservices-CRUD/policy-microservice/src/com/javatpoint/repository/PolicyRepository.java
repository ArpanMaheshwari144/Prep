package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;


import com.javatpoint.model.Policy;

public interface PolicyRepository extends CrudRepository<Policy, Integer> {
	
	@Query(nativeQuery = true, value = "select id,policies from policy")
	public List<Policy> getAllPolices();
	
	@Query(nativeQuery = true, value = "SELECT * FROM policy WHERE id = :id")
	public List<Policy> getAllPoliciesById(@Param("id") int id);
	
	@Query(nativeQuery = true, value = "SELECT * FROM policy WHERE id = :id")
	public Policy getPoliciesById(@Param("id") int id);
	
	@Query(nativeQuery = true, value = "select policies from policy")
	public List<String> getAllPolicesName();

}
