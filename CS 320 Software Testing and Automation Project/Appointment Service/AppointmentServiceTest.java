package AppointmentTest;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

import AppointmentService.AppointmentService;

class AppointmentServiceTest {

	@Test
	void addAppointmentTest() {
		AppointmentService newService = new AppointmentService();
		newService.AddNewAppointment("1", "Appointment Description", 2024, 10, 17);
		assertTrue(newService.checkForID("1"));
		
	}
	
	@Test
	void removeContactTest() {
		AppointmentService newService = new AppointmentService();
		newService.AddNewAppointment("1", "Appointment Description", 2024, 10, 17);
		newService.RemoveAppointment("1");
		assertTrue(!newService.checkForID("1"));
	}

}
