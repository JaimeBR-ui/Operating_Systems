import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;
import java.util.Set;
import java.util.Queue;
import java.util.LinkedList;

public class SantaScenario {
	// There is no reindeer present.
	public Santa santa;
	public List<Elf> elves;
	public Queue<Integer> santasDoor;
	// public List<Reindeer> reindeers;
	public boolean isDecember;
	private int day = 0;

	public int getDay() {
		return this.day;
	}

	public static void main(String args[]) {
		SantaScenario scenario = new SantaScenario();
		scenario.santasDoor = new LinkedList<Integer>();
		scenario.isDecember = false;
		// create the participants
		// Santa
		scenario.santa = new Santa(scenario);
		Thread th = new Thread(scenario.santa);
		th.start();
		// The elves: in this case: 10
		scenario.elves = new ArrayList<>();
		for(int i = 0; i != 10; i++) {
			Elf elf = new Elf(i+1, scenario);
			scenario.elves.add(elf);
			th = new Thread(elf);
			th.start();
		}
		// The reindeer: in this case: 9
		// scenario.reindeers = new ArrayList<>();
		// for(int i=0; i != 9; i++) {
		// 	Reindeer reindeer = new Reindeer(i+1, scenario);
		// 	scenario.reindeers.add(reindeer);
		// 	th = new Thread(reindeer);
		// 	th.start();
		// }
		// now, start the passing of time
		for(int day = 1; day < 500; day++) {
			scenario.day = day;
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// turn on December
			if (day > (365 - 31)) {
				scenario.isDecember = true;
			}
			// print out the state:
			System.out.println("***********  Day " + day + " *************************");
			scenario.santa.report();
			for(Elf elf: scenario.elves) {
				elf.report();
			}
			// for(Reindeer reindeer: scenario.reindeers) {
			// 	reindeer.report();
			// }
		}
	}
}
