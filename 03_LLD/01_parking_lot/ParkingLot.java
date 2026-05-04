import java.util.*;

// --- ENUMS ---
// VehicleType: BIKE, CAR, TRUCK
// SpotType: SMALL, MEDIUM, LARGE
enum VehicleType {
    BIKE, CAR, TRUCK;
}

enum SpotType {
    SMALL, MEDIUM, LARGE;
}

// --- VEHICLE (abstract class) ---
// SRP = har class ka ek hi responsibility/kaam
// Yahan kyu: Vehicle sirf data rakhta (plate, type), logic nahi
//
// OCP = class naye features ke liye OPEN, modify ke liye CLOSED
// Yahan kyu: Naya vehicle type chahiye → naya subclass, Vehicle class untouched
//
// LSP = subclass ko parent ki jagah use karo — kaam sahi chale
// Yahan kyu: Bike/Car/Truck sab Vehicle ki jagah fit (parkVehicle method mein dekh)
abstract class Vehicle {
    String licensePlate;
    VehicleType vehicleType;

    Vehicle(String licensePlate,  VehicleType vehicleType) {
        this.licensePlate = licensePlate;
        this.vehicleType = vehicleType;
    }
}

class Bike extends Vehicle {

    Bike(String licensePlate, VehicleType vehicleType) {
        super(licensePlate, vehicleType);
    }   

}

class Car extends Vehicle {

    Car(String licensePlate, VehicleType vehicleType) {
        super(licensePlate, vehicleType);
    }

    

}

class Truck extends Vehicle {

    Truck(String licensePlate, VehicleType vehicleType) {
        super(licensePlate, vehicleType);
    }

   
}

// --- PARKING SPOT ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Spot sirf apna state manage karta (park/unpark karna)
class ParkingSpot {
    String spotNumber;
    Vehicle parkedVehicle;
    boolean isAvailable;
    SpotType spotType;

    ParkingSpot(String spotNumber, Vehicle parkedVehicle, SpotType spotType, boolean isAvailable) {
        this.spotNumber = spotNumber;
        this.parkedVehicle = parkedVehicle;
        this.spotType = spotType;
        this.isAvailable = isAvailable;
    }

    String getSpotNumber() {
        return spotNumber;
    }

    SpotType getSpotType() {
        return spotType;
    }

    boolean getIsAvailable() {
        return isAvailable;
    }

    void setIsAvailable(boolean isAvailable) {
        this.isAvailable = isAvailable;
    }

    Vehicle getParkedVehicle() {
        return parkedVehicle;
    }

    void setParkedVehicle(Vehicle parkedVehicle) {
        this.parkedVehicle = parkedVehicle;
    }

    void park(Vehicle vec) {
        this.isAvailable = false;
        setParkedVehicle(vec);
    }

    void unpark(Vehicle vec) {
        this.isAvailable = true;
        setParkedVehicle(null);
    }

}

// --- TICKET ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Ticket sirf booking record hold karta (vehicle, spot, time)
class Ticket {
    Vehicle vehicle;
    ParkingSpot spot;
    String entryTime;

    Ticket(Vehicle vehicle, ParkingSpot spot, String entryTime) {
        this.vehicle = vehicle;
        this.spot = spot;
        this.entryTime = entryTime;
    }

    Vehicle getVehicle() {
        return vehicle;
    }

    void setVehicle(Vehicle vehicle) {
        this.vehicle = vehicle;
    }

    ParkingSpot getSpot() {
        return spot;
    }

    void setSpot(ParkingSpot spot) {
        this.spot = spot;
    }

    String getEntryTime() {
        return entryTime;
    }

    void setEntryTime(String entryTime) {
        this.entryTime = entryTime;
    }
}

// --- FLOOR ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Floor sirf apne spots ko manage karta — findAvailableSpot()
class Floor {
    String floorNumber;
    List<ParkingSpot> spots;

    Floor(String floorNumber) {
        this.floorNumber = floorNumber;
        this.spots = new ArrayList<>();
    }

    String getFloorNumber() {
        return floorNumber;
    }

    void setFloorNumber(String floorNumber) {
        this.floorNumber = floorNumber;
    }

    List<ParkingSpot> getSpots() {
        return spots;
    }

    void setSpots(List<ParkingSpot> spots) {
        this.spots = spots;
    }

    ParkingSpot findAvailableSpot(VehicleType vehicleType) {
        for (ParkingSpot spot : spots) {
            if (spot.getIsAvailable() && canFitVehicle(vehicleType, spot.getSpotType())) {
                return spot;
            }
        }
        return null;
    }

    private boolean canFitVehicle(VehicleType vehicleType, SpotType spotType) {
        if (vehicleType == VehicleType.BIKE && (spotType == SpotType.SMALL)) {
            return true;
        }
        if (vehicleType == VehicleType.CAR && (spotType == SpotType.MEDIUM)) {
            return true;
        }
        if (vehicleType == VehicleType.TRUCK && spotType == SpotType.LARGE) {
            return true;
        }
        return false;
    }
}

// --- PARKING LOT ---
// SRP = har class ka ek hi kaam
// Yahan kyu: ParkingLot sirf floors coordinate karta, individual spot/vehicle handle nahi
//
// Facade Pattern = complex subsystem ko simple API ke piche chhupa
// Yahan kyu: Client sirf parkVehicle()/unparkVehicle() call kare, andar Floor→Spot ka chain hide
class ParkingLot {
    List<Floor> floors;

    ParkingLot(List<Floor> floors) {
        this.floors = floors;
    }

    List<Floor> getFloors() {
        return floors;
    }

    void setFloors(List<Floor> floors) {
        this.floors = floors;
    }

    Ticket parkVehicle(Vehicle vehicle) {
        for(Floor floor : floors){
            ParkingSpot p = floor.findAvailableSpot(vehicle.vehicleType);
            if(p != null){
                p.setParkedVehicle(vehicle);
                return new Ticket(vehicle, p, "now");
            }
        }
        return null;
        
    }

    void unparkVehicle(Ticket t) {
        t.getSpot().unpark(null);
    }

}

class Main {
    public static void main(String[] args) {
        // Floor 1 — 1 SMALL, 1 MEDIUM, 1 LARGE
        Floor floor1 = new Floor("1");
        floor1.getSpots().add(new ParkingSpot("F1-S1", null, SpotType.SMALL, true));
        floor1.getSpots().add(new ParkingSpot("F1-M1", null, SpotType.MEDIUM, true));
        floor1.getSpots().add(new ParkingSpot("F1-L1", null, SpotType.LARGE, true));

        List<Floor> floors = new ArrayList<>();
        floors.add(floor1);
        ParkingLot lot = new ParkingLot(floors);

        // Bike park
        Vehicle bike = new Bike("UP32-1234", VehicleType.BIKE);
        Ticket t1 = lot.parkVehicle(bike);
        System.out.println("Bike parked at: " + (t1 != null ? t1.getSpot().getSpotNumber() : "No spot"));

        // Car park
        Vehicle car = new Car("DL01-5678", VehicleType.CAR);
        Ticket t2 = lot.parkVehicle(car);
        System.out.println("Car parked at: " + (t2 != null ? t2.getSpot().getSpotNumber() : "No spot"));

        // Bike unpark
        lot.unparkVehicle(t1);
        System.out.println("Bike unparked!");

        // Dobaara bike park
        Vehicle bike2 = new Bike("UP32-9999", VehicleType.BIKE);
        Ticket t3 = lot.parkVehicle(bike2);
        System.out.println("Bike2 parked at: " + (t3 != null ? t3.getSpot().getSpotNumber() : "No spot"));

        System.out.println("Done!");
    }
}
