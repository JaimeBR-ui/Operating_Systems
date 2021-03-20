import java.util.Random;


public class Reindeer implements Runnable {

	public enum ReindeerState {AT_BEACH, AT_WARMING_SHED, AT_THE_SLEIGH};
	private ReindeerState state;
	private SantaScenario scenario;
	private Random rand = new Random();

	/**
	 * The number associated with the reindeer
	 */
	private int number;

	public Reindeer(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ReindeerState.AT_BEACH;
	}

	@Override
	public void run() {
		while(scenario.getDay() < 370) {
		// wait a day
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// see what we need to do:
		switch(state) {
			case AT_BEACH:
				// if it is December, the reindeer might think about returning.
				if (scenario.isDecember) {
					if (rand.nextDouble() < 0.1) {
						state = ReindeerState.AT_WARMING_SHED;
					}
				}
				break;
			case AT_WARMING_SHED:
				if (scenario.atShed.getQueueLength() == 8
					&& scenario.santa.getState() == Santa.SantaState.SLEEPING)
					scenario.santa.setState(Santa.SantaState.WOKEN_UP_BY_REINDEER);

				try {
					scenario.atShed.acquire();
					this.state = ReindeerState.AT_THE_SLEIGH;
				} catch (InterruptedException ie) {
					break;
				}
				break;
			case AT_THE_SLEIGH:
				// keep pulling
				break;
			};
		}
	}

	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Reindeer " + number + " : " + state);
	}

}
