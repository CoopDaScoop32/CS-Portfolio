package Tests;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import Application.Task;

class TaskTest {

	@Test
	void TestTaskClass() {
		Task newTask = new Task("1", "Do Laundry", "Put my laundry in the washer and drier");
		assertTrue(newTask.getId().equals("1"));
		assertTrue(newTask.getName().equals("Do Laundry"));
		assertTrue(newTask.getDesc().equals("Put my laundry in the washer and drier"));
	}
	
	@Test
	void TestIdNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("10000000000", "Do Laundry", "Put my laundry in the washer and drier");
			});
	}
	
	@Test
	void TestIdNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task(null, "Do Laundry", "Put my laundry in the washer and drier");
			});
	}
	
	@Test
	void TestNameNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("1", "Go Do Laundry Right Now Or Else We Will Have Dirty Clothes Tomorrow", 
					"Put my laundry in the washer and drier");
			});
	}
	
	@Test
	void TestNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("1", null, "Put my laundry in the washer and drier");
			});
	}
	
	@Test
	void TestDescNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("1", "Do Laundry", "Put my laundry in the washer and drier and make sure to use fabric softener and bleach");
			});
	}
	
	@Test
	void TestDescNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("1", "Do Laundry", null);
			});
	}

}
