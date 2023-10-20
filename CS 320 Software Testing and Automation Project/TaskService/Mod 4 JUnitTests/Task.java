package Application;

public class Task {
	
	private String id;
	private String name;
	private String desc;
	
	public Task(String _id, String _name, String _desc) {
		
		if(_id == null || _id.length() > 10) {
			throw new IllegalArgumentException("That is not a valid ID");
		}
		if(_name == null || _name.length() > 20) {
			throw new IllegalArgumentException("That is not a valid name");
		}
		if(_desc == null || _desc.length() > 50) {
			throw new IllegalArgumentException("That is not a valid description");
		}
		
		this.id = _id;
		this.name = _name;
		this.desc = _desc;
	}
	
	//Accessors
	
	public String getId() {
		return id;
	}
	
	public String getName() {
		return name;
	}
	
	public String getDesc() {
		return desc;
	}
	
	//Mutators
	
	public void setName(String _newName) {
		name = _newName;
	}
	
	public void setDesc(String _newDesc) {
		desc = _newDesc;
	}
	

}
