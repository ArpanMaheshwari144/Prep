package com.javatpoint.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "customers")
public class Customers {
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)

	@Column(name = "id")
	private int id;

	@Column(name = "policies")
	private String policies;

	@Column(name = "claims")
	private String claims;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getPolicies() {
		return policies;
	}

	public void setPolicies(String policies) {
		this.policies = policies;
	}

	public String getClaims() {
		return claims;
	}

	public void setClaims(String claims) {
		this.claims = claims;
	}

	@Override
	public String toString() {
		return "Customers [id=" + id + ", policies=" + policies + ", claims=" + claims + "]";
	}
	
	

	

}
