package com.javatpoint.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

// ENTITY -> "product" table. Insurance product (id, name, desc, price).
@Entity
@Table(name = "product")
public class Product {

	@Id
	@Column(name = "productid")
	private Integer productId;

	@Column(name = "productname")
	private String productName;

	@Column(name = "productdesc")
	private String productDesc;

	@Column(name = "productprice")
	private Long productPrice;

	public Product(Integer productId, String productName, String productDesc, Long productPrice) {
		super();
		this.productId = productId;
		this.productName = productName;
		this.productDesc = productDesc;
		this.productPrice = productPrice;
	}

	public Product() {
		super();
		// TODO Auto-generated constructor stub
	}

	public Integer getProductId() {
		return productId;
	}

	public void setProductId(Integer productId) {
		this.productId = productId;
	}

	public String getProductName() {
		return productName;
	}

	public void setProductName(String productName) {
		this.productName = productName;
	}

	public String getProductDesc() {
		return productDesc;
	}

	public void setProductDesc(String productDesc) {
		this.productDesc = productDesc;
	}

	public Long getProductPrice() {
		return productPrice;
	}

	public void setProductPrice(Long productPrice) {
		this.productPrice = productPrice;
	}

	@Override
	public String toString() {
		return "Product [productId=" + productId + ", productName=" + productName + ", productDesc=" + productDesc
				+ ", productPrice=" + productPrice + ", getProductId()=" + getProductId() + ", getProductName()="
				+ getProductName() + ", getProductDesc()=" + getProductDesc() + ", getProductPrice()="
				+ getProductPrice() + ", getClass()=" + getClass() + ", hashCode()=" + hashCode() + ", toString()="
				+ super.toString() + "]";
	}
	
	

}
