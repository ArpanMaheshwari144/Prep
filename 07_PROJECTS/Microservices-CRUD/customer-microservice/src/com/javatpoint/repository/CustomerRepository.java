package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;


import com.javatpoint.model.Customers;

public interface CustomerRepository  extends CrudRepository<Customers, Integer> {
	
	@Query(nativeQuery = true, value = "SELECT * FROM customers")
	public List<Customers> getAllCustomers();
    
	@Query(nativeQuery = true, value = "SELECT * FROM customers WHERE id = :id")
	public List<Customers> getAllCustomersById(@Param("id") int id);
    
	@Query(nativeQuery = true, value = "SELECT customers.id ,policy.policies  FROM customers left join policy on customers.id=policy.id WHERE policy.id = :id")
	public List<Object[]> getAllCustomersPolicesById(@Param("id") int id);

	@Query(nativeQuery = true, value = "SELECT customers.id ,claim.claims  FROM customers left join claim on customers.id=claim.id WHERE claim.id = :id")
	public List<Object[]> getAllCustomersClaimsById(@Param("id") int id);
	
	
	
	

}
