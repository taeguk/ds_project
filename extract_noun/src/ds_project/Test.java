package ds_project;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;

import kr.co.shineware.nlp.komoran.core.analyzer.Komoran;
import kr.co.shineware.util.common.model.Pair;

public class Test {
	static public void main(String[] args) throws FileNotFoundException 
	{
		final String fileName = "text.txt";
		String content = new Scanner(new File(fileName)).useDelimiter("\\Z").next();
		
		Komoran komoran = new Komoran("models-full");
		List<List<Pair<String,String>>> result = komoran.analyze(content);
		for (List<Pair<String, String>> eojeolResult : result) {
			for (Pair<String, String> wordMorph : eojeolResult) {
				if ( wordMorph.getSecond().equals("NNP") || wordMorph.getSecond().equals("NNG") )
					System.out.print(wordMorph.getFirst() + " ");
			}
		}
		System.out.println("");
	}
}
