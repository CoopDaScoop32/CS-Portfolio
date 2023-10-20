package AppointmentTest;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import AppointmentService.Appointment;

class AppointmentTest {

	@Test
	void testAppointmentClass() {
		Appointment newAppointment = new Appointment("1", "This is the appointment", 2024, 10, 17);
		assertTrue(newAppointment.getId().equals("1"));
		assertTrue(newAppointment.getDesc().equals("This is the appointment"));
		assertTrue(newAppointment.getDateYear() == 2024);
		assertTrue(newAppointment.getDateMonth() == 10);
		assertTrue(newAppointment.getDateDay() == 17);

	}
	
	@Test
	void testAppointmentIdNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment(null, "This is the appointment", 2024, 10, 17);
			});
	}
	
	@Test
	void testAppointmentIdNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("29838928392903809238", "This is the appointment", 2024, 10, 17);
			});
	}
	
	
	@Test
	void testAppointmentDescNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("1", null, 2024, 10, 17);
			});
	}
	
	@Test
	void testAppointmentDescNotTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("1", "This is a very long description that will be too long and it will give an error if it is this long", 2024, 10, 17);
			});
	}
	
	@Test
	void testAppointmentDateNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("1", "This is a description", 0, 0, 0);
			});
	}

}
