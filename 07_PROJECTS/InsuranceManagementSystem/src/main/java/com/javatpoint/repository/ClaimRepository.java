package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import com.javatpoint.model.Claim;
import com.javatpoint.model.Policy;

// REPOSITORY: claim table ka DB-access. CrudRepository -> CRUD auto (save/findById/findAll/delete).
// @Query(nativeQuery=true) -> apni raw SQL. @Param -> method-arg SQL ke :id me.
public interface ClaimRepository extends CrudRepository<Claim, Integer> {


	@Query(nativeQuery = true, value = "select id,claims from claim")
	public List<Claim> getAllClaims();                          // saare claims (id, claims)

	@Query(nativeQuery = true, value = "SELECT * FROM claim WHERE id = :id")
	public List<Claim> getAllClaimsById(@Param("id") int id);   // us id ke rows (List)

	@Query(nativeQuery=true,value="SELECT * FROM claim WHERE id = :id")
	public Claim getClaimsById(@Param("id") int id);            // ek Claim (single)

}
