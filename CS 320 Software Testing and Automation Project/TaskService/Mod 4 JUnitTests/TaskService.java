package Application;
import java.util.*;

import Application.Task;
public class TaskService {

	private List<Task> Tasks = new ArrayList<Task>();
	
	public void AddNewTask(String _id, String _name, String _desc) {
		if(!checkForID(_id)) {//if Id is not found in list of tasks
			Tasks.add(new Task(_id, _name, _desc));
		}
		else {
			throw new IllegalArgumentException("That Id is already in use, please use another");
		}
	}
	
	
	public void RemoveTask(String _id) {
		Tasks.remove(findTask(_id));
	}
	
	public void EditTaskName(String _id, String _newName) {
		findTask(_id).setName(_newName);
	}
	
	public void EditTaskDesc(String _id, String _newDesc) {
		findTask(_id).setDesc(_newDesc);
	}
	
	
	public boolean checkForID(String _id){
		for(int i = 0; i < Tasks.size(); i++) {
			if(_id == Tasks.get(i).getId()) {	//if the new potential id matches an existing one, then it will throw an exception
				return true;
			}
		}
		return false;
	}
	
	public Task findTask(String _id) {
		for(int i = 0; i < Tasks.size(); i++) {
			if(_id == Tasks.get(i).getId()) {
				return Tasks.get(i);
			}
		}
		throw new IllegalArgumentException("Task not Exist");
	}
}
