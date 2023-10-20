package contactServiceTest;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contactService.Contact;
import contactService.ContactService;

class ContactServiceTest {

	@Test
	void addContactTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		assertTrue(newService.checkForID("100"));
		
	}
	
	@Test
	void removeContactTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.removeContact("100");
		assertTrue(!newService.checkForID("100"));
	}
	
	@Test
	void FindContactByIdTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.findContact("100");
		assertTrue(newService.findContact("100").getId() == "100");
	}
	
	
	@Test
	void updateContactFirstNameTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.editFirstName("100", "Bobby");
		assertTrue(newService.findContact("100").getFirstName() == "Bobby");
	}
	
	@Test
	void updateContactLastNameTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.editLastName("100", "Smith");
		assertTrue(newService.findContact("100").getLastName() == "Smith");
	}
	
	@Test
	void updateContactPhoneNumberTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.editPhoneNumber("100", "6035407786");
		assertTrue(newService.findContact("100").getPhoneNumber() == "6035407786");
	}
	
	@Test
	void updateContactEmailTest() {
		ContactService newService = new ContactService();
		newService.addNewContact("100", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		newService.editEmail("100", "boopercrien@yahoo.com");
		assertTrue(newService.findContact("100").getEmail() == "boopercrien@yahoo.com");
	}

}
