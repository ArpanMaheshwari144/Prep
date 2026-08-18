package com.javatpoint.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import com.javatpoint.model.Employee;

// REPOSITORY: employee ka DB-access. CrudRepository -> CRUD auto.
public interface EmployeeRepository extends CrudRepository<Employee, Integer>  {

	@Query(nativeQuery = true ,value="select * from employee")
	public List<Employee> getAllEmployee();   // saare employees

}
