package contactService;

public class Contact {
	
	private String id;
	private String firstName;
	private String lastName;
	private String phoneNumber;
	private String emailAddress;
	
	public Contact(String _id, String _firstName, String _lastName, String _phoneNumber, String _emailAddress) {
		if(_id == null || _id.length() > 10) {
			throw new IllegalArgumentException("Invalid id");
		}
		if(_firstName == null || _firstName.length() > 10) {
			throw new IllegalArgumentException("Invalid first name");
		}
		if(_lastName == null || _lastName.length() > 10) {
			throw new IllegalArgumentException("Invalid last name");
		}
		if(_phoneNumber == null || _phoneNumber.length() != 10) {
			throw new IllegalArgumentException("Invalid phone number");
		}
		if(_emailAddress == null || _emailAddress.length() > 30) {
			throw new IllegalArgumentException("Invalid email");
		}
		this.id = _id;
		this.firstName = _firstName;
		this.lastName = _lastName;
		this.phoneNumber = _phoneNumber;
		this.emailAddress = _emailAddress;
		
	}
	
	//Accessors
	public String getId() {
		return id;
	}
	
	public String getFirstName() {
		return firstName;
	}
	
	public String getLastName() {
		return lastName;
	}
	
	public String getPhoneNumber() {
		return phoneNumber;
	}
	
	public String getEmail() {
		return emailAddress;
	}
	
	
	//Mutators
	public void setId(String newId) {
		if(newId == null || newId.length() > 10) {
			throw new IllegalArgumentException("Invalid id");
		}
		else {
			id = newId;
		}
		
	}
	
	public void setFirstName(String newFirstName) {
		if(newFirstName == null || newFirstName.length() > 10) {
			throw new IllegalArgumentException("Invalid first name");
		}
		else {
			firstName = newFirstName;
		}
	}
	
	public void setLastName(String newLastName) {
		if(newLastName == null || newLastName.length() > 10) {
			throw new IllegalArgumentException("Invalid last name");
		}
		else {
			lastName = newLastName;
		}
	}
	
	public void setPhoneNumber(String newNumber) {
		if(newNumber == null || newNumber.length() != 10) {
			throw new IllegalArgumentException("Invalid phone number");
		}
		else {
			phoneNumber = newNumber;
		}
	}
	
	public void setEmail(String newEmail) {
		if(newEmail == null || newEmail.length() > 30) {
			throw new IllegalArgumentException("Invalid email");
		}
		else {
			emailAddress = newEmail;
		}
	}
	
	
	

}
