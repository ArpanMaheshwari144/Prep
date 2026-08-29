package com.javatpoint.service;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.feign.CustomerInterface;
import com.javatpoint.model.Customers;
import com.javatpoint.repository.CustomerRepository;


@Service
public class CustomerService {
	
	@Autowired
	CustomerInterface customerInterface;
	
	@Autowired
	CustomerRepository customerRepository;

	public List<Customers> getAllCustomers() {
		return customerRepository.getAllCustomers();
	}

	public List<Customers> getAllCustomersById(int id) {
		return customerRepository.getAllCustomersById(id);
	}

	public List<Object[]> getAllCustomersPolicesById(int id) {
		return customerRepository.getAllCustomersPolicesById(id);
	}

	public List<Object[]> getAllCustomersClaimsById(int id) {
		List<Object[]> list = new ArrayList<Object[]>();
		try {
			list = customerRepository.getAllCustomersClaimsById(id);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return list;
	}
	
	public List<String> getAllCustomersPolicesByNames() {
		return customerInterface.getAllPolicesName();
	}


}
