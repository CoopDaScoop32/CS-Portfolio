package AppointmentService;

import java.util.Date;

public class Appointment {

	
	private Date date;
	private String id = "";
	private String description = "";
	
	public Appointment(String _id, String _description, int _year, int _month, int _day) {
		Date temp_date = null;
		if(_year == 0 || _month == 0 && _day == 0) {
			temp_date = null;
		}
		else {
			temp_date = new Date(_year - 1900, _month - 1, _day);
		}
		
		if(_id == null || _id.length() > 10) {
			throw new IllegalArgumentException("Invalid id");
		}
		if(_description == null || _description.length() > 50) {
			throw new IllegalArgumentException("Invalid description");
		}
		if(temp_date == null) {
			throw new IllegalArgumentException("Date cannot be null");
		}
		if(temp_date.before(new Date())) {
			throw new IllegalArgumentException("Date cannot be before the present day");
		}
		
		id = _id;
		description = _description;
		date = temp_date;
		
		
	};
	
	
	public int getDateMonth() {
		return date.getMonth() + 1;
	}
	
	public int getDateYear() {
		return date.getYear() + 1900;
	}
	
	public int getDateDay() {
		return date.getDate();
	}
	
	public String getId() {
		return id;
	}
	
	public String getDesc() {
		return description;
	}
	
	public void setDate(int _year, int _month, int _day) {
		date = new Date(_year - 1900, _month - 1, _day);
	}
	
	public void setDesc(String _desc) {
		description = _desc;
	}
	
	
	
	
	
	
	
}
