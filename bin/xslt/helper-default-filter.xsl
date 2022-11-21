<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:param name="filter" select="'*'" />

<xsl:include href="helper-status-icons.xsl" />

<xsl:template match="/">
 <xsl:apply-templates />
</xsl:template>

<xsl:template match="*">
 <node>
   <xsl:call-template name="icon" />
  <xsl:attribute name="children"><xsl:value-of select="$filter"/></xsl:attribute>
  <xsl:value-of select="name()" />
  <xsl:for-each select="@*[name()!='diff:status']">
   <xsl:text> </xsl:text><xsl:value-of select="name()" /><xsl:text>="</xsl:text><xsl:value-of select="." /><xsl:text>"</xsl:text>
  </xsl:for-each>
  <xsl:if test="count(text()) &gt; 0"><xsl:text> = </xsl:text><xsl:value-of select="text()" /></xsl:if>
  </node>
</xsl:template>

</xsl:stylesheet>