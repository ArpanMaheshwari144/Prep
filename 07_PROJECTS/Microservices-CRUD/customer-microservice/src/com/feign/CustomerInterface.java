package com.feign;

import java.util.ArrayList;
import java.util.List;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.javatpoint.model.Policy;

@FeignClient(name="CUSTOMER-MICROSERVICE", url = "${customer.service.url}")
public interface CustomerInterface {
	
	@RequestMapping(value = "/api/policies/allnames",method = RequestMethod.GET)
    public List<String> getAllPolicesName();
}
