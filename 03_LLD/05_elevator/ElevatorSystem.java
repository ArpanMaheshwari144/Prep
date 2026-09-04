import java.util.*;

// --- Direction enum ---
enum Direction {
    UP, DOWN, IDLE;
}

// --- Elevator: id, currentFloor, direction, requests ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Elevator sirf apni state rakhta (floor, direction, requests queue)
class Elevator {
    int id, currentFloor;
    Direction direction;
    List<Integer> requests;

    public Elevator(int id, int currentFloor) {
        this.id = id;
        this.currentFloor = currentFloor;
        this.direction = Direction.UP;
        this.requests = new ArrayList<>();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCurrentFloor() {
        return currentFloor;
    }

    public void setCurrentFloor(int currentFloor) {
        this.currentFloor = currentFloor;
    }

    public Direction getDirection() {
        return direction;
    }

    public void setDirection(Direction direction) {
        this.direction = direction;
    }

    public List<Integer> getRequests() {
        return requests;
    }

    public void setRequests(List<Integer> requests) {
        this.requests = requests;
    }

}

// --- Floor: floorNumber ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Floor sirf apna number rakhta, baad mein button/sensor add karne
// ke liye extensible
class Floor {
    int floorNumber;

    public Floor(int floorNumber) {
        this.floorNumber = floorNumber;
    }

    public int getFloorNumber() {
        return floorNumber;
    }

    public void setFloorNumber(int floorNumber) {
        this.floorNumber = floorNumber;
    }
}

// --- ElevatorController: assign best elevator ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Controller sirf "best elevator kaunsa" decide karta —
// distance-based logic
class ElevatorController {
    List<Elevator> elevators;

    public ElevatorController(List<Elevator> elevators) {
        this.elevators = elevators;
    }

    public List<Elevator> getElevators() {
        return elevators;
    }

    public void setElevators(List<Elevator> elevators) {
        this.elevators = elevators;
    }

    Elevator assignElevator(int floor, Direction direction) {
        Elevator bestElevator = null;
        int minDistance = Integer.MAX_VALUE;
        for (Elevator it : elevators) {
            if (it.direction == Direction.IDLE) {
                if (minDistance > Math.abs(it.currentFloor - floor)) {
                    minDistance = Math.abs(it.currentFloor - floor);
                    bestElevator = it;
                }
            } else if (it.direction == Direction.UP && direction == Direction.UP && it.currentFloor < floor) {
                if (minDistance > Math.abs(it.currentFloor - floor)) {
                    minDistance = Math.abs(it.currentFloor - floor);
                    bestElevator = it;
                }
            } else if (it.direction == Direction.DOWN && direction == Direction.DOWN && it.currentFloor > floor) {
                if (minDistance > Math.abs(it.currentFloor - floor)) {
                    minDistance = Math.abs(it.currentFloor - floor);
                    bestElevator = it;
                }
            } else {
                continue;
            }
        }

        if(bestElevator == null){
            for (Elevator it : elevators) {
                int dist = Math.abs(it.currentFloor - floor);
                if(dist < minDistance){
                    minDistance = dist;
                    bestElevator = it;
                }
            }
        }
        return bestElevator;
    }
}

// --- Building: elevators, floors ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Building sirf elevators + floors + controller ko ek saath tie
// karta
//
// Composition = "has-a" relationship (strong ownership)
// Yahan kyu: Building HAS-A Elevators, Floors, Controller — Building khatam, in
// ka matlab nahi
//
// Facade Pattern = complex internals simple API ke piche
// Yahan kyu: Client getController() ya getElevators() call kare, construction
// detail hide
class Building {
    List<Elevator> elevators;
    List<Floor> floors;
    ElevatorController controller;

    public Building(int numElevator, int numFloor) {
        this.elevators = new ArrayList<>();
        this.floors = new ArrayList<>();
        this.controller = new ElevatorController(elevators);

        for (int i = 0; i < numElevator; i++) {
            elevators.add(new Elevator(i, 0));
        }

        for (int i = 0; i < numFloor; i++) {
            floors.add(new Floor(i));
        }
    }

    public List<Elevator> getElevators() {
        return elevators;
    }

    public List<Floor> getFloors() {
        return floors;
    }

    public ElevatorController getController() {
        return controller;
    }
}

class Main {
    public static void main(String[] args) {
        Building building = new Building(3, 10);

        ElevatorController controller = building.getController();

        // Elevator 0 floor 0, Elevator 1 floor 0, Elevator 2 floor 0
        // Set different positions
        building.getElevators().get(0).setCurrentFloor(2);
        building.getElevators().get(1).setCurrentFloor(7);
        building.getElevators().get(2).setCurrentFloor(4);

        // Floor 5 se request — sabse paas kaunsa?
        Elevator best = controller.assignElevator(5, Direction.UP);
        System.out.println(
                "Request from floor 5 → Elevator " + best.getId() + " (at floor " + best.getCurrentFloor() + ")");

        // Floor 1 se request
        Elevator best2 = controller.assignElevator(1, Direction.UP);
        System.out.println(
                "Request from floor 1 → Elevator " + best2.getId() + " (at floor " + best2.getCurrentFloor() + ")");

        // Floor 8 se request
        Elevator best3 = controller.assignElevator(8, Direction.DOWN);
        System.out.println(
                "Request from floor 8 → Elevator " + best3.getId() + " (at floor " + best3.getCurrentFloor() + ")");

        System.out.println("Elevator System Done!");
    }
}
