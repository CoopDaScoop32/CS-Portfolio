package AppointmentService;

import java.util.ArrayList;
import java.util.List;


public class AppointmentService {
	
	private List<Appointment> Appointments = new ArrayList<Appointment>();
	
	public void AddNewAppointment(String _id, String _desc, int _year, int _month, int _day) {
		if(!checkForID(_id)) {//if Id is not found in list of tasks
			Appointments.add(new Appointment(_id, _desc, _year, _month, _day));
		}
		else {
			throw new IllegalArgumentException("That Id is already in use, please use another");
		}
	}
	
	
	public void RemoveAppointment(String _id) {
		Appointments.remove(findAppointment(_id));
	}
	
	public boolean checkForID(String _id){
		for(int i = 0; i < Appointments.size(); i++) {
			if(_id == Appointments.get(i).getId()) {	//if the new potential id matches an existing one, then it will throw an exception
				return true;
			}
		}
		return false;
	}
	
	public Appointment findAppointment(String _id) {
		for(int i = 0; i < Appointments.size(); i++) {
			if(_id == Appointments.get(i).getId()) {
				return Appointments.get(i);
			}
		}
		throw new IllegalArgumentException("Appointment not Exist");
	}

}
