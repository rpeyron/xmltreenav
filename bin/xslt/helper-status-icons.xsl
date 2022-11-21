<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:template name="icon">
  <xsl:if test="@diff:status">
  <xsl:attribute name="icon">
   <xsl:choose>
    <xsl:when test="@diff:status='added'">added</xsl:when>
    <xsl:when test="@diff:status='removed'">removed</xsl:when>
    <xsl:when test="@diff:status='modified'">modified</xsl:when>
    <xsl:when test="@diff:status='below'">question</xsl:when>
    <xsl:otherwise>unknown</xsl:otherwise>
   </xsl:choose>
  </xsl:attribute>
  </xsl:if>
</xsl:template>    

</xsl:stylesheet>