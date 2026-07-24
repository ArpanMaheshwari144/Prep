package com.arpan.paymentservice;

// PayRequest — POST /pay ka request body { orderId, amount } (DTO).
//
// ★ class -> record me BADLA. KYUN?
//   - DTO sirf data CARRY karta (immutable), badalna nahi -> record perfect fit.
//   - record ek line me: constructor + accessor + equals/hashCode/toString compiler AUTO banata -> boilerplate gaya.
//   - @RequestBody se JSON aata -> Spring/Jackson record me daal deta (request-body ke liye chalega).
//   ★ NUANCE: record ka accessor "get" BINA hota -> orderId() / amount(), NA getOrderId(). isliye caller (PaymentController) bhi update kiya.
//
// -- PURANA (class version, reference ke liye) --
// public class PayRequest {
//     private String orderId;
//     private double amount;
//     public String getOrderId() { return orderId; }
//     public void setOrderId(String orderId) { this.orderId = orderId; }
//     public double getAmount() { return amount; }
//     public void setAmount(double amount) { this.amount = amount; }
// }

// ★ mera point: record class jaisa hi hai -- bas SETTERS nahi hote -> IMMUTABLE (value ek baar banti, phir badal nahi sakti).
//   getters/constructor auto ban jaate; use waise hi hota (data carry). setters isliye nahi kyunki record badalne ke liye bana hi nahi.
public record PayRequest(String orderId, double amount) {}
