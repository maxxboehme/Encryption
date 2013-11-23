/**
 * @author Maxx
 */

import java.io.*;
import java.util.*;

public class RSA {
	public static void main(String[] args) throws IOException{
		if(args.length > 0){
			if(args[0].equals("key")){
				key(Long.parseLong(args[1]), Long.parseLong(args[2]));
			}else if(args[0].equals("encrypt")){
				encryption(args[1], args[2], args[3]);
			}else if(args[0].equals("decrypt")){
				decryption(args[1], args[2], args[3]);
			}
		}else{  //print this statement if no arguments are passed
			System.out.println("No arguments passed.");
		}
	}

	public static void key(long p, long q){   //finds n e and d, then you can use d and e for encryption
		//makes sure both arguments are prime
		if(isPrime(p) && isPrime(q)) {
			long n = p*q;
			long theta = (p-1)*(q-1);
			long e = finde(n, theta);
			long d = gcd(e, theta);
			System.out.println(n+" "+e+" "+d);
		}
		else{
			System.out.println("Parameters passed are not primes. Please try again.");
		}
	}

	//finds e using RSA techniques
	public static long finde(long n, long theta){
		long result = 3; // starts at 3 since primes must be odd and greater than or equal 2
		boolean found = false;
		while(result<n && !found){
			if(gcd(result, theta+1)==1){ // checks to see if the two numbers are relatively prime
				found = true;
			}else{
				result +=2;
			}
		}
		return result;
	}

	public static void encryption(String nameInputFile, String nameKeyFile, String nameOutputFile) throws IOException{
		File input = new File(nameInputFile);
		File keyFile = new File(nameKeyFile);
		File outputFile = new File(nameOutputFile);
		if( !input.canRead() || !keyFile.canRead())
			System.out.println("One or more files is unreadable or does not exists. Please try again.");
		else {
			long[] keys = new long[3];
			Scanner fscan = new Scanner(keyFile);
			int i = 0;
			while(fscan.hasNextLong()){
				keys[i++] = fscan.nextLong();
			}
			fscan.close();

			//creates file streams for the files
			BufferedInputStream istream = new BufferedInputStream(new FileInputStream(input));
			FileOutputStream fos = new FileOutputStream(outputFile);
			DataOutputStream output = new DataOutputStream(new BufferedOutputStream(fos));

			int message = 0;
			byte[] bytes = new byte[3];
			//Maxx driving
			//checks to make sure there are still bytes left to read
			int length = 0;
			while((length = istream.read(bytes, 0, 3)) != -1){

//				System.out.println(byteAS(bytes));
//				System.out.println(byteAC(bytes));
				message = 0;
				//concatenates 3 bytes together in message
				for(int k = 0; k < length; k++){
					message = message | ((bytes[k] & 0xFF)<<(8*(2-k)));
				}
//				System.out.println("before:\t"+IntToBy(message));
				//encrypts the message using fast modular exponentiation
				message = fastModularExp(message, keys[1], keys[0]);
//				System.out.println("after:\t"+IntToBy(message)+"\n");
				//writes all 4 bytes to the output file
				output.writeInt(message);
			}
			output.close();
			istream.close();
		}
	}

	public static void clearByteArray(byte[] n){
		for(int i = 0; i < n.length; i++){
			n[i] = 0;
		}
	}

	public static String IntToBy(int n){
		String result = Integer.toBinaryString(n);
		while(result.length() < 32){
			result = "0"+result;
		}
		return result;
	}

	public static String byteAS(byte[] n){
		String result = "";
		for(int i = 0; i < n.length; i++){
			result += byteToS(n[i])+" ";
		}
		return result;
	}

	public static String byteAS(int[] n){
		String result = "";
		for(int i = 0; i < n.length; i++){
			result += IntToBy(n[i])+" ";
		}
		return result;
	}

	public static String byteAC(byte[] n){
		String result = "";
		for(int i = 0; i < n.length; i++){
			result += ((char)(n[i] & 0xFF))+" ";
		}
		return result;
	}

	public static String byteToS(byte n){
		String result = Integer.toBinaryString(n & 0xFF);
		while(result.length() < 8){
			result = "0"+result;
		}
		return result;
	}

	public static void decryption(String nameInputFile, String nameKeyFile, String nameOutputFile) throws IOException{
		//creates File objects for arguments
		File input = new File(nameInputFile);
		File keyFile = new File(nameKeyFile);
		File outputFile = new File(nameOutputFile);
		//checks to make sure files are readable and exist
		if( !input.canRead() || !keyFile.canRead())
			System.out.println("One or more files is unreadable or does not exists. Please try again.");
		else {
			long[] keys = new long[3];
			Scanner fscan = new Scanner(keyFile);
			int i = 0;
			while(fscan.hasNextLong()){
				keys[i++] = fscan.nextLong();
			}
			fscan.close();
			//creates the file streams for each file
			BufferedInputStream istream = new BufferedInputStream(new FileInputStream(input));
			FileOutputStream fos = new FileOutputStream(outputFile);
			DataOutputStream output = new DataOutputStream(new BufferedOutputStream(fos));
			int message = 0;
			byte[] bytes = new byte[4];
			int length = istream.read(bytes, 0, 4);

			//makes sure there are still bytes left to read
			while(length != -1){
//				System.out.println(byteAS(bytes));
//				System.out.println(byteAC(bytes));
				message = 0;
				//concatenates four bytes to the message
				for(int k = 0; k < length; k++){
					message = message | ((bytes[k] & 0xFF)<<(8*(3-k)));
				}
//				System.out.println("before:\t"+IntToBy(message));
				//decrypts the message using fast modular exponentiation
				message = fastModularExp(message, keys[2], keys[0]);
//				System.out.println("after:\t"+IntToBy(message)+"\n");

				//makes sure the last 2 bytes are not equal to zero
				output.writeByte((message & 16711680)>>16);
				if((length = istream.read(bytes, 0, 4)) == -1){
					if(((message & 65280)>>8)!=0){
						output.writeByte((message & 65280)>>8);
					}
					if((message & 255)!=0){
						output.writeByte(message & 255);
					}
				} else {
					output.writeByte((message & 65280)>>8);
					output.writeByte(message & 255);
				}

			}
			output.close();
			istream.close();
		}
	}

	//performs the bulk of the encryption and decryption
	public static int fastModularExp(long a, long b, long c){
		long result = 1;
		long leadingbit = Long.highestOneBit(b);
		while(leadingbit > 0){ //while there are bits left
			result = (int)((result*result)%c); //case 1: bit is a 0
			if((b & leadingbit) > 0){
				result = (int)((result*a)%c); //case 2: if bit is a 1
			}
			leadingbit = leadingbit>>1;
		}
		return (int)result;
	}

	public static long gcd(long e, long theta){
		long[] a1 = new long[3];
		long[] a2 = new long[3];
		a1[0]= 1;
		a1[2]= e;
		a2[1] = 1;
		a2[2] = theta;
		long q = 0;
		while(a2[2]!=0){  //used algorithm from class
			q = a1[2]/a2[2];
			long answer = 0;
			for(int i = 0; i<3; i++){
				answer = a1[i]-q*a2[i];  //updates triples to find gcd
				a1[i]=a2[i];
				a2[i] = answer;
			}
		}
		long result = a1[0];
		if(result<0){  //if result is negative, add theta to make positive
			result = a1[0]+theta;
		}
		return result;
	}

	public static boolean isPrime(long x) {  //checks to make sure a number is prime
		double root = Math.sqrt((double) x);
		if((int)root == root){ //quick check to see if root is an int; therefore not prime
			return false;
		}
		for(double i = 2; i < root; i++) {	//checks all possible factors from 2 to the square root of x
			double check = x / i;
			if((int)check == check)
				return false;
		}
		return true;
	}
}

