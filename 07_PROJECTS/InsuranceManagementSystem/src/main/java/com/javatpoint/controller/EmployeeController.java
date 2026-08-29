package com.javatpoint.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import com.javatpoint.model.Employee;
import com.javatpoint.service.EmployeeService;

// CONTROLLER: employee endpoints. Thin -> EmployeeService call.
@RestController
@CrossOrigin(origins = "*")
public class EmployeeController {

	@Autowired
	EmployeeService employeeService;

	// POST: naya employee create (save)
	@PostMapping("/api/create/employee")
	public void saveEmployee(@RequestBody Employee employee) {
		employeeService.saveOrUpdateEmployee(employee);
	}

	// GET: saare employees
	@GetMapping("/api/employee")
	public List<Employee> getAllEmploye() {
		return employeeService.getAllEmployee();
	}
}
