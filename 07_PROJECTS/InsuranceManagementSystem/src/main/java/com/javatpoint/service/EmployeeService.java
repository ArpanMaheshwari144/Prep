package com.javatpoint.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.javatpoint.model.Employee;
import com.javatpoint.model.Policy;
import com.javatpoint.repository.EmployeeRepository;

// SERVICE: employee logic. simple CRUD passthrough (repo pe).
@Service
public class EmployeeService {

	@Autowired
	EmployeeRepository employeeRepository;

	public List<Employee> getAllEmployee(){
		return employeeRepository.getAllEmployee();   // saare employees
	}

	// JPA save() = id nayi -> INSERT, purani -> UPDATE
	public void saveOrUpdateEmployee(Employee employee) {
		employeeRepository.save(employee);
	}

}
