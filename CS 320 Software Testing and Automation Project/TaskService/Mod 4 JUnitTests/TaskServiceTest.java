package Tests;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

import Application.TaskService;


class TaskServiceTest {

	@Test
	void addTaskTest() {
		TaskService newService = new TaskService();
		newService.AddNewTask("1", "Do Laundry", "Put my laundry in the washer and drier");
		assertTrue(newService.checkForID("1"));
		
	}
	
	@Test
	void removeTaskTest() {
		TaskService newService = new TaskService();
		newService.AddNewTask("1", "Do Laundry", "Put my laundry in the washer and drier");
		newService.RemoveTask("1");
		assertTrue(!newService.checkForID("1"));
		
	}
	
	@Test
	void findTaskById() {
		TaskService newService = new TaskService();
		newService.AddNewTask("1", "Do Laundry", "Put my laundry in the washer and drier");
		assertTrue(newService.findTask("1").getId().equals("1"));
		
	}
	
	@Test
	void editTaskNameTest() {
		TaskService newService = new TaskService();
		newService.AddNewTask("1", "Do Laundry", "Put my laundry in the washer and drier");
		newService.EditTaskName("1", "Take Out Trash");
		assertTrue(newService.findTask("1").getName().equals("Take Out Trash"));
	}
	
	@Test
	void editTaskDescTest() {
		TaskService newService = new TaskService();
		newService.AddNewTask("1", "Do Laundry", "Put my laundry in the washer and drier");
		newService.EditTaskDesc("1", "Throw trash into the dumpster");
		assertTrue(newService.findTask("1").getDesc().equals("Throw trash into the dumpster"));
	}

}
