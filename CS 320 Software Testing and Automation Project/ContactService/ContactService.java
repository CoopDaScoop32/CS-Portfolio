package contactService;
import java.util.*;
public class ContactService {
	
	private List<Contact> Contacts = new ArrayList<Contact>();
	
	public void addNewContact(String _id, String _firstName, String _lastName, String _phoneNumber, String _emailAddress) {
		if(!checkForID(_id)) {//checks if ID does not exist
			Contacts.add(new Contact(_id, _firstName, _lastName, _phoneNumber, _emailAddress));
		}
		else {
			throw new IllegalArgumentException("Invalid id");
		}
		
	}
	
	public void removeContact(String _id) {
		
		Contacts.remove(findContact(_id));
		
		
		
	}
	
	public void editFirstName(String _id, String newName) {
		findContact(_id).setFirstName(newName);
		
	}
	
	public void editLastName(String _id, String newName) {
		findContact(_id).setLastName(newName);
		
	}
	
	public void editPhoneNumber(String _id, String newNumber) {
		findContact(_id).setPhoneNumber(newNumber);
	}
	
	public void editEmail(String _id, String newEmail) {
		findContact(_id).setEmail(newEmail);
	}
	
	public Contact findContact(String _id) {
		for(int i = 0; i < Contacts.size(); i++) {
			if(_id == Contacts.get(i).getId()) {
				return Contacts.get(i);
			}
		}
		throw new IllegalArgumentException("Contact not Exist");
	}
	
	public boolean checkForID(String _id){
		for(int i = 0; i < Contacts.size(); i++) {
			if(_id == Contacts.get(i).getId()) {	//if the new potential id matches an existing one, then it will throw an exception
				return true;
			}
		}
		return false;
	}

}
