//import com.sun.org.apache.xml.internal.security.utils.HelperNodeList;


public class Santa implements Runnable {

	enum SantaState {SLEEPING, READY_FOR_CHRISTMAS, WOKEN_UP_BY_ELVES, WOKEN_UP_BY_REINDEER};
	private SantaState state;
	private SantaScenario scenario;

	public Santa(SantaScenario scenario) {
		this.state = SantaState.SLEEPING;
		this.scenario = scenario;
	}


	@Override
	public void run() {
		while(scenario.getDay() < 370) {
			// wait a day...
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			switch(state) {
			case SLEEPING: // if sleeping, continue to sleep
				break;
			case WOKEN_UP_BY_ELVES:
				// FIXME: help the elves who are at the door and go back to sleep
				while (!scenario.santasDoor.isEmpty()) {
					int elveNumber = scenario.santasDoor.remove();
					scenario.elves.get(elveNumber - 1).setState(Elf.ElfState.WORKING);
				}
				state = SantaState.SLEEPING;
				break;
			case WOKEN_UP_BY_REINDEER:
				// FIXME: assemble the reindeer to the sleigh then change state to ready
				break;
			case READY_FOR_CHRISTMAS: // nothing more to be done
				break;
			}
		}
	}

	public SantaState getState() {
		return this.state;
	}

	public void setState(SantaState s) {
		this.state = s;
	}

	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Santa : " + state);
	}


}
