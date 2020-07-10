package network;

import org.pcap4j.core.*;
import org.pcap4j.packet.EthernetPacket;
import org.pcap4j.packet.IpV4Packet;
import org.pcap4j.packet.Packet;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.util.Scanner;

public class Network {

    private static final String IPADDR = "192.168.31.17";   // the ip add for the network interface

    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        System.out.println("Please enter the number of packet you wanna capture(-1 for infinite)：");
        int count =sc.nextInt();
        InetAddress addr;
        PcapNetworkInterface nif; //the nif for capture

        try {
            addr  = InetAddress.getByName(IPADDR);
            nif = Pcaps.getDevByAddress(addr);
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        if (nif == null) {
            return;
        }
        //the object for capturing packet
        int snapLen = 65536;
        PcapNetworkInterface.PromiscuousMode mode = PcapNetworkInterface.PromiscuousMode.PROMISCUOUS;
        int timeout = 10;
        PcapHandle handle = nif.openLive(snapLen, mode, timeout);



        if(count==-1){
            while(true){
                //capture packet
                Packet packet = handle.getNextPacket();
                gotPacket(packet, IPADDR, nif);
            }
        }else {
            for (int i = 0; i <count;i++) {
                //capture packet
                Packet packet = handle.getNextPacket();
                gotPacket(packet, IPADDR, nif);

            }
        }
        handle.close();

    }

    // Working for packet
    public static void gotPacket(Packet packet, String ipAddr, PcapNetworkInterface nif) {

        //get the src IP add
        IpV4Packet ipV4Packet = packet.get(IpV4Packet.class);
        Inet4Address srcAddr = ipV4Packet.getHeader().getSrcAddr();
        System.out.println("Packet from: " + srcAddr);

        //get the head of ethernet
        EthernetPacket ethernetPacket = packet.get(EthernetPacket.class); // 以太网报文
        System.out.println(ethernetPacket.getHeader());

        //sent packet
        PcapHandle sendHandle = null;
        try {
            int snapLen = 65536;
            PcapNetworkInterface.PromiscuousMode mode = PcapNetworkInterface.PromiscuousMode.PROMISCUOUS;
            int timeout = 10;

            sendHandle = nif.openLive(snapLen, mode, timeout);

            sendHandle.sendPacket(packet);
            System.out.println("packet sent\n\n");
        } catch ( PcapNativeException e) {
            e.printStackTrace();
        } catch (NotOpenException e) {
            e.printStackTrace();
        }finally {
            sendHandle.close();
        }

    }
}
