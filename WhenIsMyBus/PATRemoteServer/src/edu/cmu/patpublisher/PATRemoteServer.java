package edu.cmu.patpublisher;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.net.HttpURLConnection;

import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Scanner;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory; //new

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.message.BasicNameValuePair;
import org.joda.time.DateTime;
import org.joda.time.Minutes;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class PATRemoteServer extends Thread {

	static boolean startDummyServer = false;
	static int Bus71A_Eta = 5;
	static int Bus71C_Eta = 10;
	static int Bus82_Eta = 15;
	public static String getDataFromPAT() throws Exception {
		// setting universal resource location (variable) website url
		URL patUrl = new URL(
				"http://realtime.portauthority.org/bustime/api/v1/getpredictions?key=zGaTW8WvTT8Mqj4SvkQPvZ6rE&stpid=8199");
		// connecting to website using openConnection() function
		HttpURLConnection conn = (HttpURLConnection) patUrl.openConnection();
		// setrequest method to actually get data...post/push
		conn.setRequestMethod("GET");
		// buffer variable to stream and store data
		BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));

		StringBuilder dataFromPAT = new StringBuilder();
		String line;
		while ((line = reader.readLine()) != null) {
			dataFromPAT.append(line);
		}
		reader.close();
		conn.disconnect();
		return dataFromPAT.toString();
	}

	public static String parseDataFromPAT(String dataFromPAT) throws Exception {

		// Get a Document Builder Factory object
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		// Get a Document Builder from the factory
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		// Create a new Input Source from PATData / takes other formats too such
		// as file
		InputSource inputSource = new InputSource(new StringReader(dataFromPAT));
		// Create a DOM (Data object model) object
		Document doc = dBuilder.parse(inputSource);
		// some sort of formatter
		doc.getDocumentElement().normalize();
		// get all nodes of prefix prd
		NodeList nList = doc.getElementsByTagName("prd");

		String modifiedData = "";
		String route, predictedTime;
		for (int nodeId = 0; nodeId < nList.getLength(); nodeId++) {
			Node nNode = nList.item(nodeId);
			if (nNode.getNodeType() == Node.ELEMENT_NODE) {
				Element eElement = (Element) nNode;
			    route = modifiedData + eElement.getElementsByTagName("rt").item(0).getTextContent();
				predictedTime = eElement.getElementsByTagName("prdtm").item(0).getTextContent();
				// modifying the data and formatting it as required. Substring function to use parts of data 
				
				
				String expectedPattern = "yyyyMMdd HH:mm";
				SimpleDateFormat formatter = new SimpleDateFormat(expectedPattern);
				Date dateBus = formatter.parse(predictedTime);
				DateTime datetimeBus = new DateTime(dateBus);
				DateTime datetimeNow = new DateTime();
				modifiedData = route + "-" + Minutes.minutesBetween(datetimeNow, datetimeBus).getMinutes() +  "\n";
			}
		}
		return modifiedData;
	}

	public static void sendToPhoton(String modifiedData) {
		HttpClient client = HttpClientBuilder.create().build();
		HttpPost post = new HttpPost("https://api.particle.io/v1/devices/events");
		try {
			List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(1);
			nameValuePairs.add(new BasicNameValuePair("name", "PATdisplay"));
			nameValuePairs.add(new BasicNameValuePair("data", modifiedData));
			nameValuePairs.add(new BasicNameValuePair("access_token", "52f3dfaa285a971bd09df2116ef7a10850a7030a"));
			post.setEntity(new UrlEncodedFormEntity(nameValuePairs));

			//to check if particle could publish it or not
			HttpResponse response = client.execute(post);
			BufferedReader rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
			String line = "";
			while ((line = rd.readLine()) != null) {
				System.out.println(line);
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void run() {
		Scanner sc = new Scanner(System.in);
		System.out.println("------Configuration Console------");
		System.out.println("------Getting Data from PAT by Default------");
		System.out.println("------Press 1 if you would like the dummy server instead------");
		while(true){
		
			try{
				int input = sc.nextInt();
				System.out.println(input);
				if(input == 1){
					startDummyServer = true;
					
				}else if(input == 0){
					startDummyServer = false;
				}
			}catch (Exception e) {
				e.printStackTrace();
				System.out.println("------Invalid Input------");
			}
			System.out.println("------Press 0 if you would like the PAT server ------");
			System.out.println("------Press 1 if you would like the dummy server instead------");
		}
		
    }

	
	public static void main(String[] args) throws Exception {
		
		//1-->Comment
		(new Thread(new PATRemoteServer())).start();
		while (true) {
			try {	
				String modifiedData;
				//--2 Uncomment Control true/false
				//startDummyServer = true;
				if(startDummyServer == true){
					if(Bus71A_Eta == 0){
						Bus71A_Eta = 15;
					}else{
						Bus71A_Eta--;
					}
					if(Bus71C_Eta == 0){
						Bus71C_Eta = 15;
					}else{
						Bus71C_Eta--;
					}
					if(Bus82_Eta == 0){
						Bus82_Eta = 15;
					}else{
						Bus82_Eta--; 
					}
					//3-- Toggle Comments
					modifiedData = "71A-"+Bus71A_Eta+"\n71C-"+Bus71C_Eta+"\n82-"+Bus82_Eta+"\n";
					//modifiedData = "71A-7\n71C-5\n82-8\n";
				}else{
					String dataFromPAT = getDataFromPAT();
					modifiedData = parseDataFromPAT(dataFromPAT);
				}
				System.out.println(modifiedData);
				sendToPhoton(modifiedData);
				//System.out.println(System.currentTimeMillis());
				Thread.sleep(15100); // 1000 milliseconds is one second. 15 secs plus 100 millisecs as buffer
			} catch (InterruptedException ex) {
				Thread.currentThread().interrupt();
			}

		}
	}
}
