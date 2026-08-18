package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import com.javatpoint.model.Claim;
import com.javatpoint.model.Customers;
import com.javatpoint.model.Policy;

// REPOSITORY: customers ka DB-access + JOIN queries (customer <-> policy/claim).
// CrudRepository -> CRUD auto. @Query(nativeQuery) -> raw SQL.
public interface CustomerRepository  extends CrudRepository<Customers, Integer> {

	@Query(nativeQuery = true, value = "SELECT * FROM customers")
	public List<Customers> getAllCustomers();                        // saare customers

	@Query(nativeQuery = true, value = "SELECT * FROM customers WHERE id = :id")
	public List<Customers> getAllCustomersById(@Param("id") int id); // us id ke customer(s)

	// LEFT JOIN customers+policy -> customer.id + uski policies. Return List<Object[]>:
	// 2 alag table ke columns mile -> kisi entity me fit nahi -> har row = Object[] {id, policies}.
	@Query(nativeQuery = true, value = "SELECT customers.id ,policy.policies  FROM customers left join policy on customers.id=policy.id WHERE policy.id = :id")
	public List<Object[]> getAllCustomersPolicesById(@Param("id") int id);

	// wahi JOIN, claim ke saath -> {id, claims}.
	@Query(nativeQuery = true, value = "SELECT customers.id ,claim.claims  FROM customers left join claim on customers.id=claim.id WHERE claim.id = :id")
	public List<Object[]> getAllCustomersClaimsById(@Param("id") int id);

}
