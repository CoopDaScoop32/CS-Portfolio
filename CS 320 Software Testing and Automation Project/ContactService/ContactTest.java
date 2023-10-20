package contactServiceTest;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contactService.Contact;

class ContactTest {

	@Test
	void testContactClass() {
		Contact newContact = new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
		assertTrue(newContact.getId().equals("10"));
		assertTrue(newContact.getFirstName().equals("Cooper"));
		assertTrue(newContact.getLastName().equals("Brien"));
		assertTrue(newContact.getPhoneNumber().equals("6037389188"));
		assertTrue(newContact.getEmail().equals("cooperbrien@gmail.com"));
	}
	
	@Test
	void testContactIdToLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("100000000000", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
			});
		
	}
	
	@Test
	void testContactIdNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact(null, "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactFirstNameToLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Coooooooooooper", "Brien", "6037389188", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactFirstNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", null, "Brien", "6037389188", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactLastNameToLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "BillyBobBobbyBrien", "6037389188", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactLastNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", null, "6037389188", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactPhoneNumberMoreThanTen() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "603738918884787", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactPhoneNumberLessThanTen() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "BillyBobBobbyBrien", "603", "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactPhoneNumberNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "BillyBobBobbyBrien", null, "cooperbrien@gmail.com");
			});
	}
	
	@Test
	void testContactEmailToLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbillybobbobbybrien@gmail.com");
			});
	}
	
	
	@Test
	void testContactEmailNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", null);
			});
	}
	
	@Test
	void testContactSetEmailNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setEmail("dngkjndksafnkljdasnfjsdafkjlsdakjlfnkjsdanfkjdsafkljnsdaf");;
			});
	}
	
	@Test
	void testContactSetEmailNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setEmail(null);;
			});
	}
	
	@Test
	void testContactSetFirstNameNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setFirstName("Cooooooooooooooooper");;
			});
	}
	
	@Test
	void testContactSetFirstNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setFirstName(null);;
			});
	}
	
	@Test
	void testContactSetLastNameNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setLastName("Cooooooooooooooooper");;
			});
	}
	
	@Test
	void testContactSetLastNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setLastName(null);;
			});
	}
	
	@Test
	void testContactSetNumberNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setPhoneNumber("847596032789");;
			});
	}
	
	@Test
	void testContactSetNumberNotTooShort() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setPhoneNumber("84");;
			});
	}
	
	@Test
	void testContactSetNumberNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("10", "Cooper", "Brien", "6037389188", "cooperbrien@gmail.com").setPhoneNumber(null);;
			});
	}

}
